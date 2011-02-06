#include <QGraphicsSceneDragDropEvent>
#include <QPainter>

#include "component.h"

namespace CutePathSim
{
  /**
   * \class Component
   * Models a component of a digital system with inputs and outputs.
   */
  Component::Component(QGraphicsItem *parent) : QGraphicsItem(parent)
  {
  }

  Component::~Component()
  {
  }

  /**
   * \fn getInputs()
   * Returns a list containing pointers to the inputs owned by the component.
   *
   * \sa getInput() getOutputs()
   */

  /**
   * \fn getOutputs()
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
    qreal totalChildHeight = 0;
    foreach(QGraphicsItem *item, childItems())
    {
      totalChildHeight += item->boundingRect().height();
    }
    qreal width = LEFT_MARGIN + maxInterfaceWidth() + RIGHT_MARGIN;
    qreal height = TOP_MARGIN + totalChildHeight + INTERFACE_MARGIN * (childItems().size() - 1) + BOTTOM_MARGIN;
    return QRect(-width / 2, -height / 2, width, height);
  }

  void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    painter->drawRect(boundingRect());
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
    repositionInterfaces();
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
    repositionInterfaces();
  }

  /**
   * \fn run()
   * Pure virtual method that is called from the component's work thread. This is where derived classes should do their processing.
   */

  /**
   * \class Component::Input
   * The Input class represents an input interface on a Component.
   */

  /**
   * Constructs an Input class with the name \a name. 
   *
   * The constructed object represents an input that accepts \a width bits, with the object \a component as the recipient.
   */
  Component::Input::Input(const QString &name, int width, Component *component) : Interface(name, component)
  {
    m_width = width;
    m_bufferSize = width / 8 + ((width % 8) ? 1 : 0);
    m_inputBuffer = new char[m_bufferSize];
    m_component = component;
  }

  Component::Input::~Input()
  {
    disconnect();
    delete m_inputBuffer;
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
  }

  /**
   * Disconnects the input.
   */
  void Component::Input::disconnect()
  {
    if(m_connection == 0)
      return;

    m_connection->m_disconnect(this);
    m_disconnect();
  }

  /**
   * Copies the data stored in the input buffer to \a buffer.
   *
   * \warning \a buffer should be at least as large as bufferSize().
   *
   * \sa bufferSize()
   */
  void Component::Input::read(char *buffer)
  {
    memcpy(buffer, m_inputBuffer, m_bufferSize);
  }

  /**
   * \fn Component::Input::bufferSize()
   * Returns the size of the input buffer used by read().
   *
   * \sa read()
   */

  /**
   * \class Component::Output
   * The Output class represents an output interface on a Component.
   */

  /**
   * Constructs an Input class with the name \a name. 
   *
   * The constructed object represents an output that sends out \a width bits, with the object \a component as the sender.
   */
  Component::Output::Output(const QString &name, int width, Component *component) : Interface(name, component)
  {
    m_width = 1;
    m_bufferSize = width / 8 + ((width % 8) ? 1 : 0);
    m_component = component;
  }

  Component::Output::~Output()
  {
    foreach(Input *input, m_connections)
    {
      input->disconnect();
    }
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
   * Connects this output to \a input.
   */
  void Component::Output::connect(Input *input)
  {
    if(input == 0)
      return;

    m_connect(input);
    input->m_connect(this);
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
      input->m_disconnect();
      m_disconnect(input);
    }
  }

  /**
   * Writes \a data to the input buffer of all of the inputs that this output is connected to.
   *
   * \warning \a data should be at least as large as bufferSize().
   *
   * \warning This method should be called for each output on each cycle, otherwise the receiving input buffers will stay the same as they were the last cycle!
   *
   * Classes calling write() should politely make sure that any remainder bits in \a data (i.e. bits greater than width() if width() % 8 != 0) are zeroed out.
   *
   * \sa bufferSize()
   */
  void Component::Output::write(const char *data)
  {
    foreach(Input *input, m_connections)
    {
      input->writeToInput(data);
    }
  }

  /**
   * \fn Component::Output::bufferSize()
   * Returns the size of the data sent by write().
   *
   * \sa write()
   */

  void Component::repositionInterfaces()
  {
    // repositions the interfaces on the scene in alphabetical order
    qreal currentY = boundingRect().y() + TOP_MARGIN;
    qreal maxWidth = maxInterfaceWidth();
    foreach(Interface *interface, m_inputs)
    {
      interface->setPos(LEFT_MARGIN + maxWidth / 2 - interface->boundingRect().width() / 2, currentY);
      currentY += interface->boundingRect().height();
      currentY += INTERFACE_MARGIN;
    }
    foreach(Interface *interface, m_outputs)
    {
      interface->setPos(LEFT_MARGIN + maxWidth / 2 - interface->boundingRect().width() / 2, currentY);
      currentY += interface->boundingRect().height();
      currentY += INTERFACE_MARGIN;
    }
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
}
