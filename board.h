#ifndef BOARD_H_
#define BOARD_H_

#include <QGraphicsScene>
#include <QHash>
#include <QList>

namespace CutePathSim
{
  class Component;

  class Board : public QGraphicsScene
  {
    Q_OBJECT

    public:
      Board(QObject *parent = 0);
      ~Board();

      void addComponent(Component *component);

    protected:
      void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
  };
}

#endif
