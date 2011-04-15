#include <QApplication>
#include <QFontMetrics>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPixmapCache>

#include "interface.h"

namespace CutePathSim
{
  /**
   * \class Interface
   * The Interface class represents an interface on a Component (either an input or an output), so that it can be displayed and manipulated on a ComponentGraph.
   *
   * This class doesn't directly manage inputs, outputs, or their connections, as that is handled by the Component::Input and Component::Output classes. The Interface class is mainly responsible for graphically drawing the interfaces, and handling drag events from the user.
   */

  /**
   * Constructs an Interface class.
   */
  Interface::Interface(const QString &name, QGraphicsItem *parent) : QGraphicsItem(parent)
  {
    m_name = name;

    /*
    // add a drop shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);
    shadow->setOffset(2, 3);
    setGraphicsEffect(shadow);
    */

    // set up the font
    if(m_font == 0)
    {
      m_font = new QFont("Helvetica");
      m_font->setPixelSize(FONT_SIZE);
    }
    m_textWidth = QFontMetrics(*m_font).size(Qt::TextSingleLine, m_name).width();

    setAcceptHoverEvents(true);
    m_drawHover = false;
  }

  Interface::~Interface()
  {
  }

  /**
   * \fn Interface::name()
   * Returns the name of the interface.
   */

  QRectF Interface::boundingRect() const
  {
    return QRect(-LEFT_MARGIN - m_textWidth / 2, -TOP_MARGIN - FONT_SIZE / 2, LEFT_MARGIN + m_textWidth + RIGHT_MARGIN, TOP_MARGIN + FONT_SIZE + BOTTOM_MARGIN);
  }

  void Interface::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
  {
    // draw a gradient background
    QLinearGradient gradient(0, 0, 0, boundingRect().height());
    if(m_drawHover)
    {
      gradient.setColorAt(0, color().lighter(110));
    }
    else
    {
      gradient.setColorAt(0, color());
    }
    gradient.setColorAt(1, Qt::white);
    QBrush gradientBrush(gradient);
    painter->setBrush(gradientBrush);
    painter->setPen(QPen(QBrush(Qt::SolidPattern), BORDER_PEN_WIDTH));
    QRect drawingRect(boundingRect().x() + BORDER_PEN_WIDTH / 2, boundingRect().y() + BORDER_PEN_WIDTH / 2, boundingRect().width() - BORDER_PEN_WIDTH, boundingRect().height() - BORDER_PEN_WIDTH);
    painter->drawRoundedRect(drawingRect, 5, 5);

    // draw the name
    // FIXME: figure out how to make the font hinting look optimal
    painter->setPen(QPen());
    painter->setFont(*m_font);
    painter->drawText(drawingRect, Qt::AlignCenter, name());
  }

  void Interface::hoverEnterEvent(QGraphicsSceneHoverEvent *)
  {
    m_drawHover = true;
    update();
  }

  void Interface::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
  {
    m_drawHover = false;
    update();
  }

  const qreal Interface::BORDER_PEN_WIDTH;
  const qreal Interface::LEFT_MARGIN;
  const qreal Interface::RIGHT_MARGIN;
  const qreal Interface::TOP_MARGIN;
  const qreal Interface::BOTTOM_MARGIN;
  const qreal Interface::FONT_SIZE;

  QFont *Interface::m_font = 0;
}
