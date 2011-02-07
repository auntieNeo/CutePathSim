#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <QGraphicsItem>

namespace CutePathSim
{
  class Interface : public QGraphicsItem
  {
    public:
      Interface(const QString &name, QGraphicsItem *parent = 0);
      ~Interface();

      QString name() { return m_name; }

      virtual QColor color() const = 0;

      QRectF boundingRect() const;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    private:
      static const qreal BORDER_PEN_WIDTH = 0;
      static const qreal LEFT_MARGIN = 20, RIGHT_MARGIN = 20, TOP_MARGIN = 10, BOTTOM_MARGIN = 10;
      static const qreal FONT_SIZE = 20;

      QString m_name;
      qreal m_textWidth;
      static QFont *m_font;
  };
}

#endif
