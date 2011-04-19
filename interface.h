#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <QGraphicsItem>

#include "common.h"

namespace CutePathSim
{
  class Interface : public QGraphicsItem
  {
    public:
      Interface(const QString &name, QGraphicsItem *parent = 0);
      virtual ~Interface();

      enum { Type = InterfaceType };

      QString name() { return m_name; }

      virtual QColor color() const = 0;

      QRectF boundingRect() const;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

      int type() const { return InterfaceType; }

      void setSelected(bool selected) { m_selected = selected; update(); }
      bool selected() { return m_selected; }

    protected:
      void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
      void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    private:
      static const qreal BORDER_PEN_WIDTH = 1;
      static const qreal SELECTED_PEN_WIDTH = 3;
      static const qreal LEFT_MARGIN = 20, RIGHT_MARGIN = 20, TOP_MARGIN = 10, BOTTOM_MARGIN = 10;
      static const qreal FONT_SIZE = 20;
      static QFont *m_font;

      QString m_name;
      qreal m_textWidth;
      bool m_drawHover;
      bool m_selected;
  };
}

#endif
