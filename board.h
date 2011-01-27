#ifndef BOARD_H_
#define BOARD_H_

#include <QGraphicsScene>
#include <QHash>
#include <QList>

namespace Kanagram
{
  class Tile;

  class Board : public QGraphicsScene
  {
    Q_OBJECT

    public:
      Board(QObject *parent = 0);
      ~Board();

      void placeTile(Tile *tile, int x, int y);

    protected:
      void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
      void setDraggingTile(Tile *tile, QPointF mousePos);

    private:
      QList<QList<Tile *> > m_tileGrid;
      QList<Tile *> m_floatingTiles;
      QPointF dragPreviousPos;  // Qt's drag events don't give you the previous position... kind of dumb
      int m_maxX, m_minX, m_maxY, m_minY;

      void growGrid(int x, int y);
  };
}

#endif
