#ifndef COMPONENT_GRAPH_H_
#define COMPONENT_GRAPH_H_

// These Graphviz headers conflict for some reason. 
//#include <graphviz/graph.h>
#include <graphviz/gvc.h>
//extern void agdeledge(Agraph_t *g, Agedge_t *e);
#include <QGraphicsItem>
#include <QPair>
#include <QSet>

#include "component.h"

namespace CutePathSim
{
  class Edge;

  class ComponentGraph : public QGraphicsItem
  {
    friend class Component::Input;
    friend class Component::Output;
    friend class GraphLayoutManager;

    public:
      ComponentGraph(Component *parent = 0);
      ~ComponentGraph();

      bool addComponent(Component *component);

      void run();

      QRectF boundingRect() const;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

      void scheduleReLayout();
      void scheduleComponentResize(Component *component) { m_resizeComponents.insert(component); scheduleReLayout(); }

      QList<Component *> const components() { return m_components.values(); }

      void layoutGraph();

//      QList<Component *>

    protected:
      void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

      void addEdge(Component::Output *from, Component::Input *to);
      void removeEdge(Component::Output *from, Component::Input *to);

      void updateItemPositions();

    private:
      Component *m_parentComponent;

      QHash<QString, Component *> m_components;
      QHash<QPair<Component::Output *, Component::Input *>, Edge *> m_edgeItems;

      QList<Component *> m_sortedComponents;

      QSet<Component *> m_resizeComponents;

      bool m_graphChanged;

      // Graphviz objects
      GVC_t *m_graphvizContext;
      Agraph_t *m_graph;
      QHash<QGraphicsItem *, Agnode_t *> m_nodes;
      QHash<QPair<Component::Output *, Component::Input *>, Agedge_t *> m_edges;

      static inline int m_agset(void *object, QString attr, QString value)
      {
        return agset(object, const_cast<char *>(qPrintable(attr)), const_cast<char *>(qPrintable(value)));
      }

      static inline QString m_agget(void *object, QString name)
      {
        return agget(object, const_cast<char *>(qPrintable(name)));
      }

      void sortGraph();
      void sortVisit(Component *component, QSet<Component*> &visited);
  };
}

#endif
