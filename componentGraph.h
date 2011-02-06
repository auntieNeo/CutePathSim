#ifndef GRAPH_H_
#define GRAPH_H_

#include <QGraphicsScene>
#include <QSet>

namespace CutePathSim
{
  class Component;

  class ComponentGraph : public QGraphicsScene
  {
    Q_OBJECT

    public:
      ComponentGraph(QObject *parent = 0);
      ~ComponentGraph();

      void addComponent(Component *component);

    protected:
      void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

      QSet<Component *> m_components;
  };
}

#endif
