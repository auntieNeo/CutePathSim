#ifndef COMMON_H_
#define COMMON_H_

#include <QEvent>

namespace CutePathSim
{
  class Component;

  const int EDGE_Z_VALUE = -1;

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
