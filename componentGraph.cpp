#include "componentGraph.h"
#include "common.h"
#include "component.h"

#include <iostream>
using namespace std;

namespace CutePathSim
{
  /**
   * \class ComponentGraph
   * The ComponentGraph class stores, arranges, and executes the components in a digital system.
   *
   * ComponentGraph inherits from QGraphicsScene. The graph can be displayed using a ComponentGraphView object.
   *
   * Components are arranged on the graph automatically using Graphviz.
   *
   * Execution of the graph is determined using a topological sorting algorithm. The result of the sorting is then executed sequentially.
   */
  ComponentGraph::ComponentGraph(QObject *parent) : QGraphicsScene(parent)
  {
    setBackgroundBrush(QBrush(Qt::white));
  }

  ComponentGraph::~ComponentGraph()
  {
  }

  void ComponentGraph::addComponent(Component *component)
  {
    if(component == 0)
      return;

    addItem(component);
    m_components.insert(component);
    // TODO: re-layout the graph with graphviz
  }

  void ComponentGraph::dragMoveEvent(QGraphicsSceneDragDropEvent * /*event*/)
  {
    cout << "inside ComponentGraph::dragMoveEvent()" << endl;
  }
}
