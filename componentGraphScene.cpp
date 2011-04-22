#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include "componentGraph.h"
#include "componentGraphScene.h"
#include "component.h"
#include "edge.h"

#include <QDebug>

#define DRAG_LINE_SIZE 2

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
    m_hoveredInterface = 0;
    m_startDragPoint = QPointF();
    m_dragLine = 0;
    m_dragging = false;
  }

  ComponentGraphScene::~ComponentGraphScene()
  {
    delete m_rootGraph;
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

    if(m_selectedInterface != 0)
    {
      m_selectedInterface->setSelected(false);
      m_selectedInterface = 0;
    }

//    Interface *interface = qgraphicsitem_cast<Interface *>(itemAt(mouseEvent->scenePos()));
    Interface *interface = dynamic_cast<Interface *>(itemAt(mouseEvent->scenePos()));
    if(interface != 0)
    {
      // initiate dragging of interface
      m_selectedInterface = interface;
      m_startDragPoint = mouseEvent->scenePos();
      m_selectedInterface->setSelected(true);
      m_dragging = true;
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
  }

  void ComponentGraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
  {
    if(m_selectedInterface != 0)
    {
      if(m_hoveredInterface != itemAt(mouseEvent->scenePos()))
      {
        if(m_hoveredInterface != 0)
          m_hoveredInterface->setSelected(false);
        if(m_selectedInterface->type() == InputType)
        {
          Component::Output *output = dynamic_cast<Component::Output *>(itemAt(mouseEvent->scenePos()));
          if(output != 0)
          {
            m_hoveredInterface = output;
            m_hoveredInterface->setSelected(true);
          }
        }
        else if(m_selectedInterface->type() == OutputType)
        {
          Component::Input *input = dynamic_cast<Component::Input *>(itemAt(mouseEvent->scenePos()));
          if(input != 0)
          {
            m_hoveredInterface = input;
            m_hoveredInterface->setSelected(true);
          }
        }
      }
      if(m_dragging)
      {
        if(m_dragLine == 0)
        {
          m_dragLine = new QGraphicsLineItem();
          m_dragLine->setPen(QPen(Qt::black, DRAG_LINE_SIZE));
          addItem(m_dragLine);
        }
        m_dragLine->setLine(QLineF(m_selectedInterface->scenePos(), mouseEvent->scenePos()));
      }
    }
    mouseEvent->setAccepted(false);
    QGraphicsScene::mouseMoveEvent(mouseEvent);
  }

  void ComponentGraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
  {
    if(m_selectedInterface != 0)
    {
      if(m_dragLine != 0)
      {
        removeItem(m_dragLine);
        delete m_dragLine;
        m_dragLine = 0;
      }
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
      m_startDragPoint = QPointF();
      if(m_hoveredInterface != 0)
      {
        m_hoveredInterface->setSelected(false);
        m_hoveredInterface = 0;
      }
    }

    m_dragging = false;

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
  }

  void ComponentGraphScene::clearRootGraph()
  {
    removeItem(m_rootGraph);
    delete m_rootGraph;  // FIXME: sweeping this problem under the rug dosen't solve anything...
    addItem(m_rootGraph = new ComponentGraph());
  }
}
