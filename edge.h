#ifndef EDGE_H_
#define EDGE_H_

#include <QGraphicsItem>
#include <QPainterPath>

#include "component.h"

namespace CutePathSim
{
  class Edge : public QGraphicsItem
  {
    public:
      Edge(Component::Output *from, Component::Input *to, QGraphicsItem *parent = 0);
      ~Edge();

      void setPath(const QPainterPath &path);

      QRectF boundingRect() const;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    private:
      class Path : public QGraphicsPathItem
      {
        public:
          Path(Edge *parent);
          ~Path();

        protected:
          void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
          void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

        private:
          Edge *m_edge;
      };

      class Arrow : public QGraphicsPathItem
      {
        public:
          Arrow(Edge *parent);
          ~Arrow();

        protected:
          void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
          void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

        private:
          Edge *m_edge;
      };

      Path *m_path;
      Arrow *m_arrow;
      Component::Output *m_from;
      Component::Input *m_to;

      bool m_drawHover;
  };
}
#endif
