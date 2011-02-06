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
    // TODO
    return QRect(0,0,0,0);
  }

  void Interface::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
  }
}
