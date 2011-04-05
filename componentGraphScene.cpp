#include "componentGraph.h"
#include "componentGraphScene.h"
#include "common.h"
#include "component.h"
#include "edge.h"

#include <iostream>
using namespace std;

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
}
