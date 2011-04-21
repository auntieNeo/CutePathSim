#ifndef COMMON_H_
#define COMMON_H_

#include <QEvent>
#include <QGraphicsItem>

#define ANIMATION_SECONDS 1
#define ANIMATION_FPS 30

namespace CutePathSim
{
  class Component;

  const int EDGE_Z_VALUE = -1;

  enum { InterfaceType = QGraphicsItem::UserType, InputType, OutputType };

  class ComponentDockEvent : public QEvent
  {
    public:
      ComponentDockEvent(Component *component) : QEvent(EventType) { m_component = component; }
      ~ComponentDockEvent() { }

      Component *component() const { return m_component; }

      static QEvent::Type EventType;

    private:
      Component *m_component;
  };
}

#endif
