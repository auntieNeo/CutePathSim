#include <QBrush>
#include <QGraphicsScene>
#include <QPen>
#include <QDebug>

#include "common.h"
#include "edge.h"

#include <iostream>
using namespace std;

namespace CutePathSim
{
  /**
   * \class
   * The Edge class represents an edge in the component graph. It draws the curve that was generated by Graphviz between nodes on the graph, along with labels and arrows. It also recieves various mouse events to allow the user to interact with the edge.
   */
  /**
   * Constructs an edge with \a from as the sending output and \a input as the receving input, and \a parent as the parent.
   */
  Edge::Edge(Component::Output *from, Component::Input *to, QGraphicsItem *parent) : QGraphicsItem(parent)
  {
    m_from = from;
    m_to = to;
    m_path = 0;
    m_drawHover = false;
    m_path = new EdgePathItem(this);
    setZValue(EDGE_Z_VALUE);
  }

  Edge::~Edge()
  {
    delete m_path;
  }

  /**
   * Sets the path used to draw the edge to \a path.
   */
  void Edge::setPath(const QPainterPath &path)
  {
    qDebug() << "entering Edge::setPath()";
    qDebug() << "pos() of the edge" << pos();
    qDebug() << "pos() of the path" << m_path->pos();
    qDebug() << "path: " << path;
    if(m_path->scene() == scene())
    {
      cout << "The scenes are the same.";
    }
    prepareGeometryChange();
    m_path->setPath(path);;
    update();
    m_path->update();
  }

  QRectF Edge::boundingRect() const
  {
    QRectF result;
    foreach(QGraphicsItem *child, childItems())
    {
      result |= child->boundingRect();
    }
    return result;
  }

  void Edge::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
  {
  }

  Edge::EdgePathItem::EdgePathItem(Edge *parent) : QGraphicsPathItem(parent)
  {
    m_edge = parent;
    setPen(QPen(QBrush(Qt::SolidPattern), 3));
    setAcceptHoverEvents(true);
  }

  Edge::EdgePathItem::~EdgePathItem()
  {
  }

  void Edge::EdgePathItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
  {
    setPen(QPen(QBrush(Qt::SolidPattern), 8));
    update();
    // FIXME: make a hover threashold so that it doesn't blink as much
  }

  void Edge::EdgePathItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
  {
    setPen(QPen(QBrush(Qt::SolidPattern), 3));
    update();
  }
}
