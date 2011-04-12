#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QPainter>
#include <QtEndian>
#include <QDebug>
#include <QCoreApplication>

#include "common.h"
#include "component.h"
#include "componentGraph.h"
#include "componentDataModel.h"
#include "mainWindow.h"

namespace CutePathSim
{
  /**
   * \class Component
   * Models a component of a digital system with inputs and outputs.
   */

  /**
   * Constructs a component with \a name as the name.
   */
  Component::Component(const QString &name, QGraphicsItem *parent) : QGraphicsItem(parent)
  {
    m_name = name;

    /*
    // add a drop shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);
    shadow->setOffset(2, 3);
    setGraphicsEffect(shadow);
    */

    // set up the font
    if(m_font == 0)
    {
      m_font = new QFont("Helvetica");
      m_font->setPixelSize(FONT_SIZE);
    }
    m_textWidth = QFontMetrics(*m_font).size(Qt::TextSingleLine, name).width();

    m_parentGraph = 0;
    m_subGraph = 0;

    m_layout = LABELED;

    m_toolBox = 0;
    m_dataTable = 0;

    setAcceptHoverEvents(true);
  }

  Component::~Component()
  {
    qDebug() << "destroying component:" << name();
    // remove all of the items from the scene and delete them
    foreach(Input *input, m_inputs)
    {
      if(input->scene() != 0)
      {
        input->scene()->removeItem(input);
      }
      delete input;
    }
    foreach(Output *output, m_outputs)
    {
      if(output->scene() != 0)
      {
        output->scene()->removeItem(output);
      }
      delete output;
    }
    if(m_subGraph != 0 && m_subGraph->scene() != 0)
    {
      m_subGraph->scene()->removeItem(m_subGraph);
    }
    delete m_subGraph;
  }

  /**
   * \fn Component::name()
   * Returns the name of the component.
   */

  /**
   * \fn Component::getInputs()
   * Returns a list containing pointers to the inputs owned by the component.
   *
   * \sa getInput() getOutputs()
   */

  /**
   * \fn Component::getOutputs()
   * Returns a list containing pointers to the outputs owned by the component.
   *
   * \sa getOutput() getInputs()
   */

  /**
   * Returns a pointer to the input associated with the \a name.
   *
   * \sa getInputs() getOutput()
   */
  Component::Input *Component::getInput(const QString &name)
  {
    return m_inputs.value(name);
  }

  /**
   * Returns a pointer to the output associated with the \a name.
   *
   * \sa getOutputs() getInput()
   */
  Component::Output *Component::getOutput(const QString &name)
  {
    return m_outputs.value(name);
  }

  QRectF Component::boundingRect() const
  {
    // FIXME: properly handle the edge case with no child items or long component names
    // find the max width and the total height of our child items, because they are interfaces that we will display in a vertical list
    switch(m_layout)
    {
      case MINIMIZED:
        {
          qreal width = LEFT_MARGIN + m_textWidth + RIGHT_MARGIN;
          qreal height = TOP_MARGIN;
          return QRect(-width / 2, -height / 2, width, height);
        }
      case LABELED:
        {
          qreal totalInterfaceHeight = 0;
          foreach(QGraphicsItem *item, m_inputs)
          {
            totalInterfaceHeight += item->boundingRect().height();
          }
          foreach(QGraphicsItem *item, m_outputs)
          {
            totalInterfaceHeight += item->boundingRect().height();
          }
          qreal width = LEFT_MARGIN + qMax(maxInterfaceWidth(), m_textWidth) + RIGHT_MARGIN;
          qreal height = TOP_MARGIN + totalInterfaceHeight + INTERFACE_MARGIN * (m_inputs.size() + m_outputs.size() - 1) + BOTTOM_MARGIN;
          return QRect(-width / 2, -height / 2, width, height);
        }
      case EXPANDED:
        {
          qreal totalInterfaceWidth = 0;
          foreach(QGraphicsItem *item, m_inputs)
          {
            totalInterfaceWidth += item->boundingRect().width();
          }
          foreach(QGraphicsItem *item, m_outputs)
          {
            totalInterfaceWidth += item->boundingRect().width();
          }
          // TODO: make the graph not fixed to a square, and handle the cascading graph changes associated with this feature
          qreal width = LEFT_MARGIN + qMax(qMax(totalInterfaceWidth + INTERFACE_MARGIN * (m_inputs.size() + m_outputs.size() - 1), m_textWidth), MIN_GRAPH_SIZE) + RIGHT_MARGIN;
          qreal graphDimensions = width - LEFT_MARGIN - RIGHT_MARGIN;
          qreal height = TOP_MARGIN + maxInterfaceHeight() + BOTTOM_MARGIN + graphDimensions + BOTTOM_MARGIN;
          return QRect(-width / 2, -height / 2, width, height);
        }
      default:
        Q_ASSERT(false);
    }
    return QRect();
  }

  void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
  {
    QRect drawingRect(boundingRect().x() + BORDER_PEN_WIDTH / 2, boundingRect().y() + BORDER_PEN_WIDTH / 2, boundingRect().width() - BORDER_PEN_WIDTH, boundingRect().height() - BORDER_PEN_WIDTH);

    // draw a gradient background
    QLinearGradient gradient(0, 0, 0, drawingRect.height());
    gradient.setColorAt(0, color());
    gradient.setColorAt(1, Qt::white);
    QBrush gradientBrush(gradient);
    painter->setBrush(gradientBrush);
    painter->setPen(QPen(Qt::NoPen));
    painter->drawRect(drawingRect);

    // draw the name
    // FIXME: figure out how to make the font hinting look optimal
    painter->setPen(QPen());
    painter->setFont(*m_font);
    painter->drawText(QRect(drawingRect.x(), drawingRect.y(), drawingRect.width(), TOP_MARGIN), Qt::AlignCenter, name());
  }

  /**
   * \fn parentGraph()
   * Returns a pointer to the ComponentGraph that this component belongs to.
   */

  /**
   * \fn subGraph()
   * Returns a pointer to the ComponentGraph that is contained within this component.
   *
   * If there is no sub-graph yet, it is constructed.
   *
   * \sa addSubComponent()
   */
  ComponentGraph *Component::subGraph()
  {
    if(m_subGraph == 0)
    {
      m_subGraph = new ComponentGraph(this);
    }
    return m_subGraph;
  }

  /**
   * \fn layout()
   * Returns the layout used to display the component, either minimized, labeled, or expanded.
   *
   * \sa setLayout() Layout
   */

  /**
   * Sets the layout used to display the component, either minimized, labeled, or expanded.
   *
   * This method initiates a re-layout of the parent graph, and the component's layout won't be changed until GraphLayoutManager is finished with the re-layout.
   *
   * \sa layout()
   */
  void Component::setLayout(Layout layout)
  {
    m_layout = layout;
    if(m_parentGraph)
    {
      m_parentGraph->scheduleComponentResize(this);
    }
  }

  void Component::mousePressEvent(QGraphicsSceneMouseEvent *)
  {
  }

  void Component::mouseDragEvent(QGraphicsSceneDragDropEvent * /*event*/)
  {
    // TODO: implement dragging of components on the graphics scene
  }

  /**
   * Adds an input to the list of inputs used by the component. Should be called from the derived class for every input it has.
   *
   * Component does \b not assume ownership of \a input. The derived class is responsible for storing inputs and outputs.
   * \sa addOutput()
   */
  void Component::addInput(Component::Input * input)
  {
    prepareGeometryChange();
    input->setParentItem(this);
    m_inputs.insert(input->name(), input);
    if(m_parentGraph != 0)
    {
      m_parentGraph->scheduleComponentResize(this);
    }
  }

  /**
   * Adds an output to the list of outputs used by the component.
   *
   * Component does \b not assume ownership of \a output. The derived class is responsible for storing inputs and outputs.
   * \sa addInput()
   */
  void Component::addOutput(Component::Output *output)
  {
    prepareGeometryChange();
    output->setParentItem(this);
    m_outputs.insert(output->name(), output);
    if(m_parentGraph != 0)
    {
      m_parentGraph->scheduleComponentResize(this);
    }
  }

  /**
   * Adds a sub-component to the component's sub-graph.
   *
   * If a sub-graph does not yet exist, it is created.
   *
   * Component (the sub-graph, really) assumes ownership of \a component. Derived classes should \b not destroy \a component.
   * \sa subGraph()
   */
  void Component::addSubComponent(Component *component)
  {
    subGraph()->addComponent(component);
  }

  /**
   * \fn run()
   * Pure virtual method that is called from the component's work thread. This is where derived classes should do their processing.
   */

  /**
   * \fn color()
   * Pure virtual method that returns the color of the component. Derived classes should return a suitable color for the component.
   */

  /**
   * \class Component::Input
   * The Input class represents an input interface on a Component.
   *
   * The input can be connected to one Output object at a time with connect(), and data sent from that output can be read with the read() method.
   *
   * \sa Output
   */

  /**
   * Constructs an Input object with the name \a name. 
   *
   * The constructed object represents an input that accepts \a width bits, with \a component as the recipient of those bits.
   */
  Component::Input::Input(const QString &name, int width, Component *component) : Interface(name, component)
  {
    m_width = width;
    m_bufferSize = width / 8 + ((width % 8) ? 1 : 0);
    m_inputBuffer = new unsigned char[m_bufferSize];
    m_component = component;
    m_connection = 0;
    m_from = 0;
    m_internal = false;
    m_externalOutput = 0;
  }

  Component::Input::~Input()
  {
    disconnect();
    delete m_inputBuffer;
    delete m_from;
  }

  /**
   * \fn Component::Input::width()
   * Returns the number of bits that the input accepts.
   */

  /**
   * \fn Component::Input::component()
   * Returns a pointer to the component this input corresponds to, the recipient of the data.
   */

  /**
   * \fn Component::Input::connection()
   * Returns a pointer to the output that is connected to this input.
   */

  /**
   * Connects this input to \a output. Any existing connection is disconnected, as inputs can only recieve from one output at a time.
   */
  void Component::Input::connect(Output *output)
  {
    disconnect();

    if(output == 0)
      return;

    output->m_connect(this);
    m_connect(output);

    // FIXME: check for connections between different graphs
    ComponentGraph *graph = internal() ? component()->subGraph() : component()->parentGraph();

    // tell the component graph and Graphviz about our new edge
    graph->addEdge(output, this);
  }

  /**
   * Disconnects the input.
   */
  void Component::Input::disconnect()
  {
    if(m_connection == 0)
      return;

    // tell the component graph and Graphviz to remove the edge
    component()->parentGraph()->removeEdge(m_connection, this);

    m_connection->m_disconnect(this);
    m_disconnect();
  }

  /**
   * Reads the data that was sent from the output that this input is connected to.
   *
   * The data is simply copied from the input buffer into \a buffer.
   *
   * \warning \a buffer should be at least as large as bufferSize().
   *
   * \sa bufferSize() readBool() readInt() Component::Output::write()
   */
  void Component::Input::read(unsigned char *buffer)
  {
    memcpy(buffer, m_inputBuffer, m_bufferSize);
  }

  /**
   * \fn Component::Input::readBool()
   * Convenience method that reads a boolean value from the input buffer.
   *
   * \warning This should not be used unless width() == 1.
   *
   * \sa read() readInt() Output::writeBool()
   */

  /**
   * Convenience method that returns an unsigned int value from the input buffer.
   *
   * One can specify whether or not to interpret the integer stored in the buffer as big endian or little endian by changing the \a bigEndian flag. If true, the first byte in the input buffer will be used as the most significant byte in the integer, and the last byte as the least significant. If false, the first byte will be used as the least significant, and the last byte as the most significant. Endianness of the returned integer depends on the system.
   *
   * \warning This should not be used unless width() == 32.
   *
   * \sa read() readBool()
   */
  unsigned int Component::Input::readInt(bool bigEndian)
  {
    Q_ASSERT(width() == 32);
    if(bigEndian)
    {
      return qFromBigEndian<qint32>(m_inputBuffer);
    }
    return qFromLittleEndian<qint32>(m_inputBuffer);
  }

  /**
   * \fn Component::Input::bufferSize()
   * Returns the size in bytes of the input buffer used by read().
   *
   * \sa read()
   */

  /**
   * Returns a pointer to the internal Output object which allows sub-components on the sub-graph to utilize this input.
   *
   * Returns null if this input is an internal input on a component's sub-graph.
   *
   * \sa internal() Component::addSubComponent()
   */
  Component::Output *Component::Input::from()
  {
    if(m_internal)
      return 0;

    if(m_from == 0)
    {
      m_from = new Output("from " + name(), width(), component());  // TODO: figure out how to translate this
//      m_from->setParentItem(component()->subGraph());  // FIXME: this doesn't work for some reason
      m_from->m_internal = true;
    }

    return m_from;
  }

  /**
   * \fn Component::Input::internal()
   * Returns true if this input is an internal input on a component's sub-graph. Otherwise, returns false.
   *
   * \sa from() Component::Output::internal()
   */

  void Component::Input::writeToInput(const unsigned char *data)
  {
    memcpy(m_inputBuffer, data, m_bufferSize);

    // forward the data to the internal output, if we have one
    if(m_from != 0)
    {
      m_from->write(data);
    }
    // forward the data to the external output, if we're an internal input
    if(m_internal)
    {
      m_externalOutput->write(data);
    }
  }

  /**
   * \class Component::Output
   * The Output class represents an output interface on a Component. 
   *
   * The output can be connected to any number of Input objects with connect(), and it will send data to those inputs whenever write() is called.
   *
   * \sa Input
   */

  /**
   * Constructs an Output object with the name \a name. 
   *
   * The constructed object represents an output that sends out \a width bits, with \a component as the sender of those bits.
   */
  Component::Output::Output(const QString &name, int width, Component *component) : Interface(name, component)
  {
    m_width = width;
    m_bufferSize = width / 8 + ((width % 8) ? 1 : 0);
    m_component = component;
    m_to = 0;
    m_internal = false;
  }

  Component::Output::~Output()
  {
    foreach(Input *input, m_connections)
    {
      input->disconnect();
    }
    delete m_to;
  }

  /**
   * \fn Component::Output::width()
   * Returns the number of bits that the output sends out.
   */

  /**
   * \fn Component::Output::component()
   * Returns a pointer to the component this output corresponds to, the sender of the data.
   */

  /**
   * Connects this output to \a input. Disconnects \a input from anything if it is already connected.
   */
  void Component::Output::connect(Input *input)
  {
    if(input == 0)
      return;

    input->disconnect();

    m_connect(input);
    input->m_connect(this);

    // FIXME: check for connections between different graphs
    ComponentGraph *graph = internal() ? component()->subGraph() : component()->parentGraph();

    // tell the component graph and Graphviz about our new edge
    graph->addEdge(this, input);
  }

  /**
   * Disconnects \a input from this output.
   */
  void Component::Output::disconnect(Input *input)
  {
    if(input == 0)
      return;

    if(m_connections.contains(input))
    {
      // tell the component graph and Graphviz to remove the edge
      component()->parentGraph()->removeEdge(this, input);

      input->m_disconnect();
      m_disconnect(input);
    }
  }

  /**
   * Writes \a data to all of the inputs that this output is connected to.
   *
   * \a data is simply copied to the input buffers of all of the connected inputs.
   *
   * \warning \a data should be at least as large as bufferSize().
   *
   * \warning This method should be called for each output on each cycle, otherwise the receiving input buffers will stay the same as they were the last cycle!
   *
   * Classes calling write() should politely make sure that any remainder bits in \a data (i.e. bits greater than width() if width() % 8 != 0) are zeroed out.
   *
   * \sa bufferSize() Component::Input::read()
   */
  void Component::Output::write(const unsigned char *data)
  {
    foreach(Input *input, m_connections)
    {
      input->writeToInput(data);
    }
  }

  /**
   * Convenience method that writes a boolean value to all of the inputs connected to this output.
   *
   * \warning This should not be used unless width() == 1.
   *
   * \sa write() writeInt() Input::readBool()
   */
  void Component::Output::writeBool(bool boolean)
  {
    Q_ASSERT(width() == 1);
    write(reinterpret_cast<unsigned char *>(&boolean));
  }

  /**
   * Convenience method that writes an integer value to all of the inputs connected to this output.
   *
   * One can specify whether or not to write the integer to the inputs in big endian or little endian byte order by changing the \a bigEndian flag. If true, the most significant byte will be written first, and the least significant byte last. If false, the least significant byte will be written first, and the most significant byte last. Endianness of \a integer should be in the system's native byte order.
   *
   * \warning This should not be used unless width() == 32.
   *
   * \sa write() writeBool() Input::readInt()
   */
  void Component::Output::writeInt(unsigned int integer, bool bigEndian)
  {
    Q_ASSERT(width() == 32);
    unsigned char buffer[sizeof(unsigned int)];
    if(bigEndian)
    {
      qToBigEndian(integer, buffer);
    }
    else
    {
      qToLittleEndian(integer, buffer);
    }
    write(buffer);
  }

  /**
   * \fn Component::Output::bufferSize()
   * Returns the size in bytes of the data sent by write().
   *
   * \sa write()
   */

  /**
   * Returns a pointer to the internal Input object which allows sub-components on the sub-graph to utilize this output.
   *
   * Returns null if this output is an internal output on a component's sub-graph.
   *
   * \sa internal() Component::addSubComponent()
   */
  Component::Input *Component::Output::to()
  {
    if(m_internal)
      return 0;

    if(m_to == 0)
    {
      // FIXME: this shouldn't be done here...
      m_to = new Input("to " + name(), width(), component());  // TODO: figure out how to translate this
//      m_to->setParentItem(component()->subGraph());  // FIXME: this doesn't work for some reason
      m_to->m_internal = true;
      m_to->m_externalOutput = this;
    }

    return m_to;
  }

  /**
   * \fn Component::Output::internal()
   * Returns true if this output is an internal output on a component's sub-graph. Otherwise, returns false.
   *
   * \sa to() Component::Input::internal()
   */

  void Component::repositionInterfaces()
  {
    // TODO: rename this routine to something more appropriate
    switch(m_layout)
    {
      case MINIMIZED:
        {
          // remove the interfaces from the scene as we're not displaying them
          foreach(Input *input, m_inputs)
          {
            input->setParentItem(0);
            if(input->scene() != 0)
            {
              input->scene()->removeItem(input);
            }
          }
          foreach(Output *output, m_outputs)
          {
            output->setParentItem(0);
            if(output->scene() != 0)
            {
              output->scene()->removeItem(output);
            }
          }
          // remove the graph from the scene as we're not displaying it
          if(m_subGraph != 0)
          {
            m_subGraph->setParentItem(0);
            if(m_subGraph->scene() != 0)
            {
              m_subGraph->scene()->removeItem(m_subGraph);
            }
          }
        }
        break;
      case LABELED:
        {
          // add the interfaces back to the scene
          foreach(Input *input, m_inputs)
          {
            input->setParentItem(this);
          }
          foreach(Output *output, m_outputs)
          {
            output->setParentItem(this);
          }
          // repositions the interfaces into a vertical list in alphabetical order
          qreal currentY = boundingRect().y() + TOP_MARGIN;
          foreach(Interface *interface, m_inputs)
          {
            if(interface->scene() != scene())
            {
              scene()->addItem(interface);  // add the item back into the scene
            }
            interface->setPos(0, currentY + interface->boundingRect().height() / 2);
            currentY += interface->boundingRect().height();
            currentY += INTERFACE_MARGIN;
          }
          foreach(Interface *interface, m_outputs)
          {
            if(interface->scene() != scene())
            {
              scene()->addItem(interface);  // add the item back into the scene
            }
            interface->setPos(0, currentY + interface->boundingRect().height() / 2);
            currentY += interface->boundingRect().height();
            currentY += INTERFACE_MARGIN;
          }
          // remove the graph from the scene as we're not displaying it
          if(m_subGraph != 0)
          {
            m_subGraph->setParentItem(0);
            if(m_subGraph->scene() != 0)
            {
              m_subGraph->scene()->removeItem(m_subGraph);
            }
          }
        }
        break;
      case EXPANDED:
        {
          // add the interfaces back to the scene
          foreach(Input *input, m_inputs)
          {
            input->setParentItem(this);
          }
          foreach(Output *output, m_outputs)
          {
            output->setParentItem(this);
          }
          // repositions the interfaces into a horizontal list at the top, with the sub-graph underneath them
          // FIXME: center the interfaces
          QRectF boundingRect = this->boundingRect();
          qreal currentX = boundingRect.x() + LEFT_MARGIN;
          qreal maxInterfaceHeight = this->maxInterfaceHeight();
          foreach(Interface *interface, m_inputs)
          {
            if(interface->scene() != scene())
            {
              scene()->addItem(interface);  // add the item back into the scene
            }
            interface->setPos(currentX + interface->boundingRect().width() / 2, boundingRect.y() + TOP_MARGIN + maxInterfaceHeight / 2);
            currentX += interface->boundingRect().width();
            currentX += INTERFACE_MARGIN;
          }
          foreach(Interface *interface, m_outputs)
          {
            if(interface->scene() != scene())
            {
              scene()->addItem(interface);  // add the item back into the scene
            }
            interface->setPos(currentX + interface->boundingRect().width() / 2, boundingRect.y() + TOP_MARGIN + maxInterfaceHeight / 2);
            currentX += interface->boundingRect().width();
            currentX += INTERFACE_MARGIN;
          }
          // add the graph back to the scene so it can be displayed
          if(m_subGraph != 0)
          {
            m_subGraph->setParentItem(this);
          }
          // scale the graph so that it fits on the component
          // FIXME: scale this whenever the graph changes
          qreal graphDimensions = boundingRect.width() - LEFT_MARGIN - RIGHT_MARGIN;
          m_subGraph->setScale(graphDimensions / qMax(m_subGraph->boundingRect().width(), m_subGraph->boundingRect().height()));
//          m_subGraph->setScale(0.25);
          m_subGraph->setPos(LEFT_MARGIN - boundingRect.width() / 2, TOP_MARGIN + maxInterfaceHeight + BOTTOM_MARGIN - boundingRect.height() / 2);
        }
        break;
      default:
        ;
    }
  }

  void Component::prepareGeometryChange()
  {
    if(m_parentGraph)
    {
      m_parentGraph->scheduleComponentResize(this);
    }
  }

  /**
   * Returns an instance of the toolbox containing all of the tools needed to manipulate the component. Derived component classes can reimplement this method to add tools to the toolbox as they see fit.
   *
   * As the toolbox can be destroyed by closeToolBox(), this method should construct a new toolbox if no constructed toolbox exists. If a constructed toolbox does exist, a pointer to that toolbox should be returned.
   *
   * This method retains the ownership of the toolbox returned.
   *
   * \sa closeToolBox()
   */
  QToolBox *Component::getToolBox()
  {
    if(m_toolBox == 0)
    {
      m_toolBox = new QToolBox();
      m_toolBox->setWindowTitle(name());
      m_dataTable = new QTableView(m_toolBox);
      m_dataTable->setModel(new ComponentDataModel(this));
      m_toolBox->addItem(m_dataTable, QObject::tr("Component Data"));
    }
    return m_toolBox;
  }

  /**
   * Closes and frees memory associated with the toolbox. Derived classes should reimplement this method if they have made changes to the toolbox that need to be cleaned up.
   *
   * Because keeping a toolbox for each and every component would be a prohibitively large waste of memory, the toolbox can be destroyed at any time with this method.
   *
   * \sa getToolBox()
   */
  void Component::closeToolBox()
  {
    delete m_toolBox;
    m_toolBox = 0;
    if(m_dataTable != 0)
    {
      QAbstractItemModel *model = m_dataTable->model();
      delete m_dataTable;
      delete model;
    }
    m_dataTable = 0;
  }

  qreal Component::maxInterfaceWidth() const
  {
    // returns the width of the widest interface in the component
    qreal maxInterfaceWidth = 0;
    foreach(QGraphicsItem *item, m_inputs)
    {
      if(item->boundingRect().width() > maxInterfaceWidth)
      {
        maxInterfaceWidth = item->boundingRect().width();
      }
    }
    foreach(QGraphicsItem *item, m_outputs)
    {
      if(item->boundingRect().width() > maxInterfaceWidth)
      {
        maxInterfaceWidth = item->boundingRect().width();
      }
    }
    return maxInterfaceWidth;
  }

  qreal Component::maxInterfaceHeight() const
  {
    // returns the height of the tallest interface in the component
    qreal maxInterfaceHeight = 0;
    foreach(QGraphicsItem *item, m_inputs)
    {
      if(item->boundingRect().height() > maxInterfaceHeight)
      {
        maxInterfaceHeight = item->boundingRect().height();
      }
    }
    foreach(QGraphicsItem *item, m_outputs)
    {
      if(item->boundingRect().height() > maxInterfaceHeight)
      {
        maxInterfaceHeight = item->boundingRect().height();
      }
    }
    return maxInterfaceHeight;
  }

  void Component::hoverEnterEvent(QGraphicsSceneHoverEvent *)
  {
  }

  void Component::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
  {
  }

  void Component::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
  {
    QCoreApplication::sendEvent(mainWindow, new ComponentDockEvent(this));
  }

  const qreal Component::MIN_GRAPH_SIZE;  // FIXME: why does this need a definition and the others don't?

  QFont *Component::m_font = 0;
}
