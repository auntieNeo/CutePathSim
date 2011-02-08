#include "componentGraph.h"
#include "common.h"
#include "component.h"

#define POINTS_IN_INCH 72

#include <iostream>
using namespace std;

// FIXME: Remove this crap. Graphviz' header file was broken so I had to copy the agDELedge routine from the library's source code. <_<
#define TAG_NODE                        1
#define TAG_EDGE                        2
#define TAG_GRAPH                       3
#define TAG_OF(p)                       (((Agraph_t*)(p))->tag)

Agdict_t *agdictof(void *obj)
{
    Agdict_t *d = NULL;

    switch (TAG_OF(obj)) {
    case TAG_GRAPH:
        d = ((Agraph_t *) obj)->univ->globattr;
        break;
    case TAG_NODE:
        d = ((Agnode_t *) obj)->graph->univ->nodeattr;
        break;
    case TAG_EDGE:
        d = ((Agedge_t *) obj)->tail->graph->univ->edgeattr;
        break;
    }
    return d;
}

void agFREEedge(Agedge_t * e)
{
  int i, nobj;
  Agdict_t *dict = agdictof(e);

  dict = dict;
  TAG_OF(e) = -1;
  nobj = dtsize(e->tail->graph->univ->edgeattr->dict);
  for (i = 0; i < nobj; i++)
    agstrfree(e->attr[i]);
  free(e->attr);
  free(e->didset);
  free(e);
}

void agDELedge(Agraph_t * g, Agedge_t * e)
{
  Agraph_t *meta;
  Agraph_t *g0;
  Agedge_t *f;

/*
  if (dtsearch(g->inedges, e) == NULL) {
    agerr(AGERR, "Edge %p was not found\n", e);
    dtwalk(g->inedges, printedge, NIL(void *));
    return;
  }
*/
  if (AG_IS_METAGRAPH(g) == FALSE) {
    meta = g->meta_node->graph;
    for (f = agfstout(meta, g->meta_node); f; f = agnxtout(meta, f)) {
      g0 = agusergraph(f->head);
      if (dtsearch(g0->inedges, e))
        agDELedge(g0, e);
    }
  }
  dtdelete(g->inedges, e);
  dtdelete(g->outedges, e);
  if (g == g->root)
    agFREEedge(e);
}

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
    agnodeattr(m_graph, const_cast<char*>("fixedsize"), const_cast<char*>("true"));
    agnodeattr(m_graph, const_cast<char*>("shape"), const_cast<char*>("box"));
    agnodeattr(m_graph, const_cast<char*>("width"), const_cast<char*>("1"));
    agnodeattr(m_graph, const_cast<char*>("height"), const_cast<char*>("1"));
//    agnodeattr(m_graph, const_cast<char*>("pos"), const_cast<char*>("42"));

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
    component->m_componentGraph = this;

    // make a Graphviz node
    m_nodes.insert(component, agnode(m_graph, const_cast<char *>(qPrintable(component->name()))));

    // TODO: re-layout the graph with Graphviz
    return true;
  }

  /**
   * \fn prepareLayoutGraph()
   * Tells the component graph to re-layout the graph when it next gets the chance.
   *
   * This is needed because laying out the graph is a relatively expensive operation, and changes to the graph should be applied collectively in one layout operation.
   */

  // FIXME: find some hook with which to actually layout the graph <_<
  void ComponentGraph::layoutGraph()  // TODO: make this private
  {
    // set the size of all of the components
    // FIXME: figure out how to do this only when the component size changes
    foreach(Component *component, m_components)
    {
      // FIXME: component's boundingRect() is already inefficient... fix it, or don't call it so much
      m_agset(m_nodes.value(component), "width", QVariant(component->boundingRect().width() / POINTS_IN_INCH).toString());
      m_agset(m_nodes.value(component), "height", QVariant(component->boundingRect().height() / POINTS_IN_INCH).toString());
    }

    gvLayout(m_graphvizContext, m_graph, "dot");
    gvRender(m_graphvizContext, m_graph, "dot", 0);

    // set the positions of the components with the new layout information
    foreach(Component *component, m_components)
    {
      // FIXME: change this to use agxget
      if(agget(m_nodes.value(component), "pos") == 0)
      {
        cout << "the pos is null" << endl;
      }
      else
      {
        cout << "The pos of " << component->name().toStdString() << ": " << agget(m_nodes.value(component), "pos") << endl;
      }
      cout << "The width of " << component->name().toStdString() << ": " << agget(m_nodes.value(component), "width") << endl;
      cout << "The height of " << component->name().toStdString() << ": " << agget(m_nodes.value(component), "height") << endl;
      cout << "m_components.size(): " << m_components.size() << endl;
      QString point;
      QList<QString> splitPoint = QString(agget(m_nodes.value(component), "pos")).split(",");
      component->setX(QVariant(splitPoint[0]).toFloat());
      component->setY(QVariant(splitPoint[1]).toFloat());
    }

    gvFreeLayout(m_graphvizContext, m_graph);
  }

  void ComponentGraph::dragMoveEvent(QGraphicsSceneDragDropEvent * /*event*/)
  {
    cout << "inside ComponentGraph::dragMoveEvent()" << endl;
  }

  /**
   * Adds an edge to the list of edges so that Graphviz knows about it.
   *
   * This is called from either Component::Output or Component::Input when they create new connections.
   */
  void ComponentGraph::addEdge(Component::Output *from, Component::Input *to)
  {
    Q_ASSERT(m_nodes.contains(from->component()));
    Q_ASSERT(m_nodes.contains(to->component()));

    QPair<Component::Output *, Component::Input *> key(from, to);

    Q_ASSERT(!m_edges.contains(key));

    cout << "adding an edge from " << from->component()->name().toStdString() << " to " << to->component()->name().toStdString() << endl;
    m_edges.insert(key, agedge(m_graph, m_nodes[from->component()], m_nodes[to->component()]));
  }

  /**
   * Remove an edge to the list of edges.
   *
   * This is called from either Component::Output or Component::Input when they disconnect.
   */
  void ComponentGraph::removeEdge(Component::Output *from, Component::Input *to)
  {
    Q_ASSERT(m_nodes.contains(from->component()));
    Q_ASSERT(m_nodes.contains(to->component()));

    QPair<Component::Output *, Component::Input *> key(from, to);

    Q_ASSERT(m_edges.contains(key));

    agDELedge(m_graph, m_edges.take(key));
  }
}
