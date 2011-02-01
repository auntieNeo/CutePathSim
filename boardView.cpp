#include "boardView.h"

namespace CutePathSim
{
  BoardView::BoardView(QWidget *parent) : QGraphicsView(parent)
  {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  }

  BoardView::~BoardView()
  {
  }

  void BoardView::resizeEvent(QResizeEvent *event)
  {
    fitInView(sceneRect(), Qt::KeepAspectRatio);
  }
}
