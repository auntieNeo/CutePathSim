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
  }

  ComponentGraphView::~ComponentGraphView()
  {
  }

  void ComponentGraphView::resizeEvent(QResizeEvent *event)
  {
    fitInView(sceneRect(), Qt::KeepAspectRatio);
  }
}
