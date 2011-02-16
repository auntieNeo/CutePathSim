#include <QDebug>
#include <QGraphicsScene>

#include "componentGraph.h"
#include "common.h"
#include "component.h"
#include "edge.h"

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

    m_layoutGraph = false;

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
    }
  }

  ComponentGraph::~ComponentGraph()
  {
    // free graphviz graph and context
    agclose(m_graph);
    gvFreeContext(m_graphvizContext);

    // FIXME: If the component graph isn't currently in a scene, delete all of the child components here. See Component::~Component().
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
    prepareGeometryChange();
    // set the size of all of the nodes
    // FIXME: figure out how to do this only when the items change size
    foreach(QGraphicsItem *item, m_nodes.keys())
    {
      // FIXME: stop calling these bounding rects so much
      m_agset(m_nodes.value(item), "width", QVariant(item->boundingRect().width() / POINTS_IN_INCH).toString());
      m_agset(m_nodes.value(item), "height", QVariant(item->boundingRect().height() / POINTS_IN_INCH).toString());
    }

    if(m_parentComponent)
    {
      gvLayout(m_graphvizContext, m_graph, "dot");
      gvRenderFilename(m_graphvizContext, m_graph, "png", "dotTest.png");
      gvRender(m_graphvizContext, m_graph, "dot", 0);
    }
    else
    {
      gvLayout(m_graphvizContext, m_graph, "dot");
      gvRender(m_graphvizContext, m_graph, "dot", 0);
    }

    // set the positions of the nodes with the new layout information
    foreach(QGraphicsItem *item, m_nodes.keys())
    {
      if(agget(m_nodes.value(item), "pos") == 0)
      {
//        cout << "the pos is null" << endl;
      }
      else
      {
        QString point;
        QList<QString> splitPoint = QString(agget(m_nodes.value(item), "pos")).split(",");  // FIXME: change this to use agxget
        item->setX(QVariant(splitPoint[0]).toFloat());
        item->setY(QVariant(splitPoint[1]).toFloat());
      }
    }

    // set the curves of the edges
    QHashIterator<QPair<Component::Output *, Component::Input *>, Agedge_t *> edgesIterator(m_edges);
    while(edgesIterator.hasNext())
    {
      edgesIterator.next();
      Agedge_t *edge = edgesIterator.value();
      if(agget(edge, "pos") == 0)
      {
//        cout << "The edge pos is null" << endl;
      }
      else
      {
        // parse the points from the spline string representation of the edge
        QList<QPointF> splinePoints;
        QPointF endPoint;  // FIXME: also get a start point, and determine when they're used
        foreach(QString pointString, QString(agget(edge, "pos")).split(" "))
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
        // adding the following line works
//        addItem(new QGraphicsPathItem(path));
      }
    }

    gvFreeLayout(m_graphvizContext, m_graph);

    qDebug() << "m_nodes.keys().size():" << m_nodes.keys().size();
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

    cout << "adding an edge from " << fromNodeName.toStdString() << " to " << toNodeName.toStdString() << endl;

    m_edges.insert(key, agedge(m_graph, m_nodes[fromNode], m_nodes[toNode]));
    Edge *edgeItem = new Edge(from, to, this);
    m_edgeItems.insert(key, edgeItem);
    // TODO: layout graph
  }

  /**
   * Remove an edge to the list of edges.
   *
   * This is called from either Component::Output or Component::Input when they disconnect.
   */
  void ComponentGraph::removeEdge(Component::Output *from, Component::Input *to)
  {
    // FIXME: update this to be aware of internal inputs/outputs
    Q_ASSERT(m_nodes.contains(from->component()));
    Q_ASSERT(m_nodes.contains(to->component()));

    QPair<Component::Output *, Component::Input *> key(from, to);

    Q_ASSERT(m_edges.contains(key));

    Edge *edgeItem = m_edgeItems.take(key);
    scene()->removeItem(edgeItem);
    delete edgeItem;

    agDELedge(m_graph, m_edges.take(key));
    // TODO: layout graph
  }
}
