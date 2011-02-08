#ifndef GRAPH_H_
#define GRAPH_H_

// These Graphviz headers conflict for some reason. 
//#include <graphviz/graph.h>
#include <graphviz/gvc.h>
extern void agdeledge(Agraph_t *g, Agedge_t *e);
#include <QGraphicsScene>
#include <QPair>
#include <QSet>

#include "component.h"

namespace CutePathSim
{
  class ComponentGraph : public QGraphicsScene
  {
    Q_OBJECT

    friend class Interface;

    public:
      ComponentGraph(QObject *parent = 0);
      ~ComponentGraph();

      bool addComponent(Component *component);

      void prepareLayoutGraph() { m_layoutGraph = true; }

    protected:
      void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

      void addEdge(Component::Output *from, Component::Input *to);
      void removeEdge(Component::Output *from, Component::Input *to);

      QHash<QString, Component *> m_components;

      // Graphviz objects
      GVC_t *m_graphvizContext;
      Agraph_t *m_graph;
      QHash<Component *, Agnode_t *> m_nodes;
      QHash<QPair<Component::Output *, Component::Input *>, Agedge_t *> m_edges;

      bool m_layoutGraph;
  };
}

#endif