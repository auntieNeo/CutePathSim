#include "board.h"
#include "common.h"
#include "tile.h"

#include <iostream>
using namespace std;

namespace Kanagram
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

  void Board::placeTile(Tile *tile, int x, int y)
  {
    growGrid(x, y);

    // TODO: assert that the tile can be placed here
    tile->setX(BOARD_MARGIN + (x - m_minX) * TILE_WIDTH + (qMax(x - m_minX - 1, 0)) * TILE_MARGIN);
    tile->setY(BOARD_MARGIN + (y - m_minY) * TILE_WIDTH + (qMax(y - m_minY - 1, 0)) * TILE_MARGIN);
    tile->setGridX(x);
    tile->setGridY(y);
    m_tileGrid[x][y] = tile;
    addItem(tile);

    // scale the scene, if needed
    if(x > m_maxX)
      m_maxX = x;
    else if(x < m_minX)
      m_minX = x;
    if(y > m_maxY)
      m_maxY = y;
    else if(y < m_maxY)
      m_minY = y;

    setSceneRect(m_minX * TILE_WIDTH - BOARD_MARGIN, m_minY * TILE_HEIGHT - BOARD_MARGIN, (-m_minX + m_maxX + 1) * TILE_WIDTH + (-m_minX + m_maxX) * TILE_MARGIN + 2 * BOARD_MARGIN, (-m_minY + m_maxY + 1) * TILE_WIDTH + (-m_minY + m_maxY) * TILE_MARGIN + 2 * BOARD_MARGIN + TILE_HOLDER_HEIGHT);
  }

  void Board::dragMoveEvent(QGraphicsSceneDragDropEvent * /*event*/)
  {
    cout << "inside Board::dragMoveEvent()" << endl;
  }

  void Board::growGrid(int x, int y)
  {
    // grow the grid to at least the point x, y... should be used whenever accessing the grid
    // FIXME: this could probably be safer if it was inside some fancy operator[] of a custom class
    if(m_tileGrid.size() <= x)
    {
      for(int i = m_tileGrid.size(); i <= x; i++)
      {
        m_tileGrid.append(QList<Tile *>());
      }
    }
    if(m_tileGrid[x].size() <= y)
    {
      for(int i = m_tileGrid[x].size(); i <= y; i++)
      {
        m_tileGrid[x].append(0);
      }
    }
  }
}
