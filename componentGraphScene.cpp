#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include "componentGraph.h"
#include "componentGraphScene.h"
#include "component.h"
#include "edge.h"

#include <QDebug>

namespace CutePathSim
{
  /**
   * \class ComponentGraphScene
   * The ComponentGraphScene class is responsible for managing all of the graphics items in the component graph and its sub-graphs. ComponentGraphScene contains the root graph.
   *
   * ComponentGraphScene inherits from QGraphicsScene. The graph can be displayed using a ComponentGraphSceneView object.
   *
   * Components are arranged on the graph automatically using Graphviz. ComponentGraphScene is responsible for calling on Graphviz to layout the root graph whenever the graph changes.
   *
   * ComponentGraphScene is responsible for the inital execution of the graph. Execution of the graph is determined using a topological sorting algorithm. The result of the sorting is then executed sequentially.
   */
  ComponentGraphScene::ComponentGraphScene(QObject *parent) : QGraphicsScene(parent)
  {
    setBackgroundBrush(QBrush(Qt::white));
    addItem(m_rootGraph = new ComponentGraph());

    m_selectedInterface = 0;
    m_startDragPoint = QPointF();
    m_dragInterface = 0;
  }

  ComponentGraphScene::~ComponentGraphScene()
  {
  }

  /**
   * Adds \a component to the root component graph.
   *
   * If an existing component in the graph has the same name as \a component, then \a component is not added and this returns false. Otherwise, returns true and adds the component.
   */
  bool ComponentGraphScene::addComponent(Component *component)
  {
    return m_rootGraph->addComponent(component);
  }

  void ComponentGraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
  {
    if (mouseEvent->button() != Qt::LeftButton)
      return;

//    Interface *interface = qgraphicsitem_cast<Interface *>(itemAt(mouseEvent->scenePos()));
    Interface *interface = dynamic_cast<Interface *>(itemAt(mouseEvent->scenePos()));
    if(interface != 0)
    {
      // initiate dragging of interface
      m_selectedInterface = interface;
      m_startDragPoint = mouseEvent->scenePos();
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
  }

  void ComponentGraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
  {
    if(m_selectedInterface != 0)
    {
      if(m_dragInterface == 0)
      {
        addItem(m_dragInterface = new DragInterface(m_selectedInterface));
      }
      m_dragInterface->move(mouseEvent->pos() - mouseEvent->lastPos());
      m_dragInterface->update();
      qDebug() << "dragged interface...";
    }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
  }

  void ComponentGraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
  {
    qDebug() << "made it this far";
    if(m_selectedInterface != 0)
    {
      qDebug() << "selectedInterface is non-zero";
//      Interface *interface = qgraphicsitem_cast<Interface *>(itemAt(mouseEvent->scenePos()));
      Interface *interface = dynamic_cast<Interface *>(itemAt(mouseEvent->scenePos()));
      if(interface != 0 && interface != m_selectedInterface)
      {
//        Component::Output *selectedOutput = qgraphicsitem_cast<Component::Output *>(m_selectedInterface);
        Component::Output *selectedOutput = dynamic_cast<Component::Output *>(m_selectedInterface);
        if(selectedOutput != 0)
        {
          Q_ASSERT(m_selectedInterface->type() == OutputType && m_selectedInterface->type() != InputType);
//          Component::Input *dropInput = qgraphicsitem_cast<Component::Input*>(interface);
          Component::Input *dropInput = dynamic_cast<Component::Input*>(interface);
          if(dropInput != 0)
          {
            ComponentGraph *outputGraph = selectedOutput->internal() ? selectedOutput->component()->subGraph() : selectedOutput->component()->parentGraph();
            ComponentGraph *inputGraph = dropInput->internal() ? dropInput->component()->subGraph() : dropInput->component()->parentGraph();
            if(outputGraph == inputGraph)
            {
              selectedOutput->connect(dropInput);
            }
          }
        }
        else
        {
//          Component::Input *selectedInput = qgraphicsitem_cast<Component::Input *>(m_selectedInterface);
          Component::Input *selectedInput = dynamic_cast<Component::Input *>(m_selectedInterface);
          if(selectedInput != 0)
          {
//            Component::Output *dropOutput = qgraphicsitem_cast<Component::Output*>(interface);
            Component::Output *dropOutput = dynamic_cast<Component::Output*>(interface);
            if(dropOutput != 0)
            {
              ComponentGraph *inputGraph = selectedInput->internal() ? selectedInput->component()->subGraph() : selectedInput->component()->parentGraph();
              ComponentGraph *outputGraph = dropOutput->internal() ? dropOutput->component()->subGraph() : dropOutput->component()->parentGraph();
              if(outputGraph == inputGraph)
              {
                selectedInput->connect(dropOutput);
              }
            }
          }
        }
      }
      else if(m_startDragPoint != mouseEvent->scenePos())
      {
        // TODO: animate interface back to the start of the drag
      }
      m_selectedInterface = 0;
      m_startDragPoint = QPointF();
      delete m_dragInterface;
      m_dragInterface = 0;
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
  }

  DragInterface::DragInterface(Interface *interface, QGraphicsItem *parent) : QGraphicsItem(parent)
  {
    m_fromInterface = interface;
    m_offset = QPointF(0, 0);
  }

  DragInterface::~DragInterface()
  {
  }

  void DragInterface::move(QPointF offset)
  {
    prepareGeometryChange();
    m_offset += offset;
  }

  QRectF DragInterface::boundingRect() const
  {
    return m_fromInterface->boundingRect().united(QRectF(m_offset + m_fromInterface->pos(), m_offset + m_fromInterface->pos()));
  }

  void DragInterface::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    painter->setPen(QPen(QBrush(Qt::SolidPattern), 4));
    painter->drawLine(m_fromInterface->pos(), m_offset + m_fromInterface->pos());
  }
}
