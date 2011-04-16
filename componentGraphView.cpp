#include <QGLWidget>
#include <QKeyEvent>

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
    /*
    QGLFormat fmt;
    fmt.setSampleBuffers(true);
    fmt.setSamples(2);
    setViewport(new QGLWidget(fmt));
    */
    setFocusPolicy(Qt::StrongFocus);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    m_zoomInAction = new QAction(tr("Zoom &In"), this);
    m_zoomInAction->setShortcut(QKeySequence("+"));
    // FIXME: make sure this shortcut doesn't steal focus from widgets that need the + symbol
//    m_zoomInAction->setShortcutContext(Qt::WidgetShortcut);
    m_zoomOutAction = new QAction(tr("Zoom &Out"), this);
    m_zoomOutAction->setShortcut(QKeySequence("-"));
//    m_zoomOutAction->setShortcutContext(Qt::WidgetShortcut);
    m_fitViewAction = new QAction(tr("&Fit View"), this);
    m_fitViewAction->setCheckable(true);
//    m_fitViewAction->setShortcutContext(Qt::WidgetShortcut);

    connect(m_zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(m_zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));
    connect(m_fitViewAction, SIGNAL(toggled(bool)), this, SLOT(m_setFitView(bool)));

    setFitView(true);
  }

  ComponentGraphView::~ComponentGraphView()
  {
  }

  void ComponentGraphView::setFitView(bool fitView)
  {
    m_fitViewAction->setChecked(fitView);
  }

  void ComponentGraphView::zoomIn()
  {
    m_fitViewAction->setChecked(false);
    scale(1 + ZOOM_FACTOR, 1 + ZOOM_FACTOR);
  }

  void ComponentGraphView::zoomOut()
  {
    m_fitViewAction->setChecked(false);
    scale(1 - ZOOM_FACTOR, 1 - ZOOM_FACTOR);
  }

  void ComponentGraphView::resizeEvent(QResizeEvent *)
  {
    if(isFitView())
    {
      QRect fitRect(sceneRect().x() - FIT_MARGIN, sceneRect().y() - FIT_MARGIN, sceneRect().width() + FIT_MARGIN * 2, sceneRect().height() + FIT_MARGIN * 2);
      fitInView(fitRect, Qt::KeepAspectRatio);
    }
  }

  void ComponentGraphView::keyPressEvent(QKeyEvent *event)
  {
    QGraphicsView::keyPressEvent(event);
  }

  void ComponentGraphView::m_setFitView(bool fitView)
  {
    if(fitView)
    {
      setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      resizeEvent(0);
    }
    else
    {
      setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
      setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
  }
}
