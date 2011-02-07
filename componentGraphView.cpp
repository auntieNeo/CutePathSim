#include "componentGraphView.h"

namespace CutePathSim
{

  /**
   * \class ComponentGraphView
   * The ComponentGraphView class is used to display a ComponentGraph to the user.
   *
   * The only difference right now between ComponentGraphView and QGraphicsView is that ComponentGraphView will automatically resize the graph to fit the view.
   */

  /**
   * Constructs a ComponentGraphView.
   */
  ComponentGraphView::ComponentGraphView(QWidget *parent) : QGraphicsView(parent)
  {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  }

  ComponentGraphView::~ComponentGraphView()
  {
  }

  void ComponentGraphView::resizeEvent(QResizeEvent *)
  {
    QRect fitRect(sceneRect().x() - FIT_MARGIN, sceneRect().y() - FIT_MARGIN, sceneRect().width() + FIT_MARGIN * 2, sceneRect().height() + FIT_MARGIN * 2);
    fitInView(fitRect, Qt::KeepAspectRatio);
  }
}
