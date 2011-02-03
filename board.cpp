#include "board.h"
#include "common.h"

#include <iostream>
using namespace std;

namespace CutePathSim
{
  Board::Board(QObject *parent) : QGraphicsScene(parent)
  {
    setBackgroundBrush(QBrush(Qt::darkGreen));
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
