#ifndef BOARD_VIEW_H_
#define BOARD_VIEW_H_

#include <QGraphicsView>

namespace Kanagram
{
  class BoardView : public QGraphicsView
  {
    Q_OBJECT

    public:
      BoardView(QWidget *parent = 0);
      ~BoardView();

    protected:
      void resizeEvent(QResizeEvent *event);
  };
}

#endif
