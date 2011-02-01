#include "board.h"
#include "common.h"
#include "tile.h"

#include <iostream>
using namespace std;

namespace CutePathSim
{
  Board::Board(QObject *parent) : QGraphicsScene(parent)
  {
    setBackgroundBrush(QBrush(Qt::darkGreen));
    m_maxX = 1;
    m_minX = -1;
    m_maxY = 1;
    m_minY = -1;

//    placeTile(new Tile(0, 0), 0, 0);
    addItem(new Tile(0, 0));
  }

  Board::~Board()
  {
    // TODO: destroy all the graphics items, because Qt apparently doesn't do that for us
  }

  void Board::addComponent(Component *component)
  {
    // TODO
  }

  void Board::dragMoveEvent(QGraphicsSceneDragDropEvent * /*event*/)
  {
    cout << "inside Board::dragMoveEvent()" << endl;
  }
}
