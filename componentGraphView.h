#ifndef COMPONENT_GRAPH_VIEW_H_
#define COMPONENT_GRAPH_VIEW_H_

#include <QGraphicsView>
#include <QAction>

namespace CutePathSim
{
  class ComponentGraphView : public QGraphicsView
  {
    Q_OBJECT

    public:
      ComponentGraphView(QWidget *parent = 0);
      ~ComponentGraphView();

      bool isFitView() { return m_fitViewAction->isChecked(); }

      QAction *zoomInAction() { return m_zoomInAction; }
      QAction *zoomOutAction() { return m_zoomOutAction; }
      QAction *fitViewAction() { return m_fitViewAction; }

    public slots:
      void setFitView(bool fitView);
      void zoomIn();
      void zoomOut();

    protected:
      void resizeEvent(QResizeEvent *event);
      void keyPressEvent(QKeyEvent *event);

    private:
      static const qreal FIT_MARGIN = 50, ZOOM_FACTOR = 0.35;
      QAction *m_zoomInAction, *m_zoomOutAction, *m_fitViewAction;

    private slots:
      void m_setFitView(bool fitView);
  };
}

#endif
