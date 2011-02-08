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

    // initilize graphviz context and graph
    m_graphvizContext = gvContext();
    m_graph = agopen(const_cast<char *>("graph"), AGDIGRAPH);  // non-strict, directed graph

    m_layoutGraph = false;
  }

  ComponentGraph::~ComponentGraph()
  {
    // free graphviz graph and context
    agclose(m_graph);
    gvFreeContext(m_graphvizContext);
  }

  /**
   * Adds \a component to the graph.
   *
   * If an existing component in the graph has the same name as \a component, then \a component is not added and this returns false. Otherwise, returns true and adds the component.
   */
  bool ComponentGraph::addComponent(Component *component)
  {
    Q_ASSERT(component != 0);

    // make sure component names are unique
    if(m_components.contains(component->name()))
    {
      return false;
    }
    addItem(component);
    m_components.insert(component->name(), component);

    // make a Graphviz node
    m_nodes.insert(component, agnode(m_graph, const_cast<char *>(qPrintable(component->name()))));

    // TODO: re-layout the graph with Graphviz
    return true;
  }

  // FIXME: find some hook with which to actually layout the graph <_<
  /**
   * \fn prepareLayoutGraph()
   * Tells the component graph to re-layout the graph when it next gets the chance.
   *
   * This is needed because laying out the graph is a relatively expensive operation, and changes to the graph should be applied collectively in one layout operation.
   */

  void ComponentGraph::dragMoveEvent(QGraphicsSceneDragDropEvent * /*event*/)
  {
    cout << "inside ComponentGraph::dragMoveEvent()" << endl;
  }

  void ComponentGraph::addEdge(Component::Output *from, Component::Input *to)
  {
    // Adds an edge to the list of edges so that Graphviz knows about it.
    // This is called from either Component::Output or Component::Input when they are connected.
    Q_ASSERT(m_nodes.contains(from->component()));
    Q_ASSERT(m_nodes.contains(to->component()));

    QPair<Component::Output *, Component::Input *> key(from, to);

    Q_ASSERT(!m_edges.contains(key));

    m_edges.insert(key, agedge(m_graph, m_nodes[from->component()], m_nodes[to->component()]));
  }

  void ComponentGraph::removeEdge(Component::Output *from, Component::Input *to)
  {
    // Remove an edge to the list of edges.
    // This is called from either Component::Output or Component::Input when they are disconnected.
    Q_ASSERT(m_nodes.contains(from->component()));
    Q_ASSERT(m_nodes.contains(to->component()));

    QPair<Component::Output *, Component::Input *> key(from, to);

    Q_ASSERT(m_edges.contains(key));

    agdeledge(m_graph, m_edges.take(key));
  }
}
