#include <QDebug>
#include <QGraphicsScene>

#include "componentGraph.h"
#include "common.h"
#include "component.h"
#include "edge.h"
#include "graphLayoutManager.h"

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
   * Components are arranged on the graph automatically using Graphviz. ComponentGraph manages the graph's representation in Graphviz' library, and positions its items with the dot graphing algorithm. Items are positioned within an approximately square aspect ratio.
   *
   * Execution of the graph is determined using a topological sorting algorithm. The result of the sorting is then executed sequentially.
   */
  ComponentGraph::ComponentGraph(Component *parent) : QGraphicsItem(parent)
  {
    // initilize graphviz context and graph
    m_graphvizContext = gvContext();
    m_graph = agopen(const_cast<char *>("graph"), AGDIGRAPH);  // non-strict, directed graph
    agnodeattr(m_graph, const_cast<char*>("fixedsize"), const_cast<char*>("true"));
    agnodeattr(m_graph, const_cast<char*>("shape"), const_cast<char*>("box"));
    agnodeattr(m_graph, const_cast<char*>("width"), const_cast<char*>("1"));
    agnodeattr(m_graph, const_cast<char*>("height"), const_cast<char*>("1"));
    agset(m_graph, const_cast<char*>("aspect"), const_cast<char*>("1"));
    agset(m_graph, const_cast<char*>("overlap"), const_cast<char*>("prism"));
    agset(m_graph, const_cast<char*>("overlap_scaling"), const_cast<char*>("0"));
//    agnodeattr(m_graph, const_cast<char*>("pos"), const_cast<char*>("42"));

    m_parentComponent = parent;

    if(m_parentComponent != 0)
    {
      // make Graphviz nodes for the internal inputs/outputs
      foreach(Component::Input *input, m_parentComponent->getInputs())
      {
        m_nodes.insert(input->from(), agnode(m_graph, const_cast<char *>(qPrintable(input->from()->name()))));
        input->from()->setParentItem(this);
      }
      foreach(Component::Output *output, m_parentComponent->getOutputs())
      {
        m_nodes.insert(output->to(), agnode(m_graph, const_cast<char *>(qPrintable(output->to()->name()))));
        output->to()->setParentItem(this);
      }
      // TODO: add/remove the internal input/output nodes whenever the inputs/outputs are changed
    }

    m_graphChanged = true;
    m_halted = false;
  }

  ComponentGraph::~ComponentGraph()
  {
    // destroy all all the components manually before the graphviz stuff is destroyed
    foreach(Component *component, m_components.values())
    {
      if(scene() != 0)
      {
        scene()->removeItem(component);
      }
      delete component;
    }

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
    component->setParentItem(this);
    m_components.insert(component->name(), component);
    component->setParentGraph(this);

    // make a Graphviz node
    m_nodes.insert(component, agnode(m_graph, const_cast<char *>(qPrintable(component->name()))));

    scheduleComponentResize(component);
    scheduleReLayout();

    m_graphChanged = true;

    return true;
  }

  /**
   * Runs the components in the component graph using a topological sort to determine order.
   */
  void ComponentGraph::run()
  {
    if(m_graphChanged)
    {
      sortGraph();
      m_graphChanged = false;
    }

    if(m_parentComponent != 0)
      qDebug() << "running graph for " << m_parentComponent->name();
    qDebug() << "size of m_sortedComponents:" << m_sortedComponents.size();
    qDebug() << "size of m_components:" << m_components.size();
    foreach(Component *component, m_sortedComponents)
    {
      qDebug() << component->name();
    }
    foreach(Component *component, m_sortedComponents)
    {
      // TODO: check for sensitive inputs
      component->run();
    }
  }

  void ComponentGraph::layoutGraph()
  {
    prepareGeometryChange();
    // update the size of the nodes for the components that changed size
    foreach(Component *component, m_resizeComponents)
    {
      component->repositionInterfaces();
      m_agset(m_nodes.value(component), "width", QVariant(component->boundingRect().width() / POINTS_IN_INCH).toString());
      m_agset(m_nodes.value(component), "height", QVariant(component->boundingRect().height() / POINTS_IN_INCH).toString());
    }
    if(m_parentComponent != 0)
    {
      // update the size of the interfaces
      foreach(Component::Input *input, m_parentComponent->getInputs())
      {
        m_agset(m_nodes.value(input->from()), "width", QVariant(input->from()->boundingRect().width() / POINTS_IN_INCH).toString());
        m_agset(m_nodes.value(input->from()), "height", QVariant(input->from()->boundingRect().height() / POINTS_IN_INCH).toString());
      }
      foreach(Component::Output *output, m_parentComponent->getOutputs())
      {
        m_agset(m_nodes.value(output->to()), "width", QVariant(output->to()->boundingRect().width() / POINTS_IN_INCH).toString());
        m_agset(m_nodes.value(output->to()), "height", QVariant(output->to()->boundingRect().height() / POINTS_IN_INCH).toString());
      }
    }

    gvLayout(m_graphvizContext, m_graph, "dot");
    gvRender(m_graphvizContext, m_graph, "dot", 0);

    // set the positions of the nodes with the new layout information
    foreach(QGraphicsItem *item, m_nodes.keys())
    {
      QString point;
      QList<QString> splitPoint = QString(m_agget(m_nodes.value(item), "pos")).split(",");  // FIXME: change this to use agxget
      item->setX(QVariant(splitPoint[0]).toFloat());
      item->setY(QVariant(splitPoint[1]).toFloat());
    }

    // set the curves of the edges
    QHashIterator<QPair<Component::Output *, Component::Input *>, Agedge_t *> edgesIterator(m_edges);
    while(edgesIterator.hasNext())
    {
      edgesIterator.next();
      Agedge_t *edge = edgesIterator.value();
      // parse the points from the spline string representation of the edge
      QList<QPointF> splinePoints;
      QPointF endPoint;  // FIXME: also get a start point, and determine when they're used
      foreach(QString pointString, QString(m_agget(edge, "pos")).split(" "))
      {
        QList<QString>values = pointString.split(",");
        if(values[0] == "e")
        {
          endPoint = QPointF(QVariant(values[1]).toFloat(), QVariant(values[2]).toFloat());
        }
        else
        {
          splinePoints.append(QPointF(QVariant(values[0]).toFloat(), QVariant(values[1]).toFloat()));
        }
      }
      // convert the spline points into a bezier path
      QPainterPath path;
      path.moveTo(splinePoints[0]);
      // draw curve points
      for(int i = 1; i < splinePoints.size(); i += 3)
      {
        path.cubicTo(splinePoints[i], splinePoints[i + 1], splinePoints[i + 2]);
      }
      // draw end point
      path.lineTo(endPoint);
      // set the edge item's path
      m_edgeItems.value(edgesIterator.key())->setPath(path);
    }

    gvFreeLayout(m_graphvizContext, m_graph);

    if(m_parentComponent != 0)
    {
      m_parentComponent->repositionInterfaces();  // the component graph's dimensions changed, so we need to reposition the parent component's layout
    }
  }

  QRectF ComponentGraph::boundingRect() const
  {
    return childrenBoundingRect();
  }

  void ComponentGraph::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
  {
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
    QString fromNodeName = from->internal() ? from->name() : from->component()->name();
    QString toNodeName = to->internal() ? to->name() : to->component()->name();
    QGraphicsItem *fromNode = from->internal() ? reinterpret_cast<QGraphicsItem *>(from) : from->component();
    QGraphicsItem *toNode = to->internal() ? reinterpret_cast<QGraphicsItem *>(to) : to->component();
    Q_ASSERT(m_nodes.contains(fromNode));
    Q_ASSERT(m_nodes.contains(toNode));

    QPair<Component::Output *, Component::Input *> key(from, to);

    Q_ASSERT(!m_edges.contains(key));

    m_edges.insert(key, agedge(m_graph, m_nodes[fromNode], m_nodes[toNode]));
    Edge *edgeItem = new Edge(from, to, this);
    m_edgeItems.insert(key, edgeItem);

    scheduleReLayout();

    m_graphChanged = true;
  }

  /**
   * Remove an edge to the list of edges.
   *
   * This is called from either Component::Output or Component::Input when they disconnect.
   */
  void ComponentGraph::removeEdge(Component::Output *from, Component::Input *to)
  {
    QGraphicsItem *fromNode = from->internal() ? reinterpret_cast<QGraphicsItem *>(from) : from->component();
    QGraphicsItem *toNode = to->internal() ? reinterpret_cast<QGraphicsItem *>(to) : to->component();
    Q_ASSERT(m_nodes.contains(fromNode));
    Q_ASSERT(m_nodes.contains(toNode));

    QPair<Component::Output *, Component::Input *> key(from, to);

    Q_ASSERT(m_edges.contains(key));
    Q_ASSERT(m_edgeItems.contains(key));

    Edge *edgeItem = m_edgeItems.take(key);
    scene()->removeItem(edgeItem);
    delete edgeItem;

    agDELedge(m_graph, m_edges.take(key));

    scheduleReLayout();

    m_graphChanged = true;
  }

  /**
   * Schedules the graph for a re-layout. The graph will be layed out in a seperate thread using Graphviz.
   * \sa scheduleComponentResize()
   */
  void ComponentGraph::scheduleReLayout()
  {
    GraphLayoutManager::instance()->scheduleLayoutGraph(this);
  }

  /**
   * \fn scheduleComponentResize()
   * Puts \a component in a queue so it can be resized when the graph is next re-layed out.
   * Also schedules a re-layout of the graph.
   */

  void ComponentGraph::sortGraph()
  {
    // use a topological sort to determine execution order
    m_sortedComponents.clear();
    QList<Component *> currentComponents;
    QSet<Component *> visitedComponents;
    foreach(Component *component, m_components)
    {
      // add components with no connected outputs, ignoring edges that connect to internal inputs, into currentNodes
      bool hasOutgoingEdges = false;
      foreach(Component::Output *output, component->getOutputs())
      {
        if(output->connections().size() == 0)
        {
          hasOutgoingEdges = false;
          break;
        }
        foreach(Component::Input *input, output->connections())
        {
          if(!input->internal())
          {
            hasOutgoingEdges = true;
            break;
          }
        }
        if(hasOutgoingEdges)
        {
          break;
        }
      }
      if(!hasOutgoingEdges)
      {
        qDebug() << "noOutgoing:" << component->name();
        currentComponents.append(component);
      }
    }
    // visit all of the current nodes
    while(!currentComponents.isEmpty())
    {
      Component *current = currentComponents.takeLast();
      sortVisit(current, visitedComponents);
    }
  }

  void ComponentGraph::sortVisit(Component *component, QSet<Component *> &visited)
  {
    if(visited.contains(component))
      return;

    visited.insert(component);
    foreach(Component::Input *input, component->getInputs())
    {
      if(input->connection() == 0 || input->connection()->internal())
      {
        continue;
      }

      sortVisit(input->connection()->component(), visited);
    }
    m_sortedComponents.append(component);
  }
}
