#ifndef TEST_COMPONENT_H_
#define TEST_COMPONENT_H_

#include "component.h"

namespace CutePathSim
{
  class TestComponent : public Component
  {
    public:
      TestComponent(const QString &name, QGraphicsItem *parent = 0);
      ~TestComponent();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Input *m_input_01, *m_input_02, *m_multiplyFlag;
      Output *m_output;
  };
}

#endif