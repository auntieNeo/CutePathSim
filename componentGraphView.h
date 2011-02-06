#ifndef COMPONENT_GRAPH_VIEW_H_
#define COMPONENT_GRAPH_VIEW_H_

#include <QGraphicsView>

namespace CutePathSim
{
  class ComponentGraphView : public QGraphicsView
  {
    Q_OBJECT

    public:
      ComponentGraphView(QWidget *parent = 0);
      ~ComponentGraphView();

    protected:
      void resizeEvent(QResizeEvent *event);

    private:
      static const qreal FIT_MARGIN = 50;
  };
}

#endif
