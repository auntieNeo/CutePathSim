#ifndef GRAPH_H_
#define GRAPH_H_

#include <QGraphicsScene>
#include <QHash>
#include <QList>

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
  };
}

#endif
