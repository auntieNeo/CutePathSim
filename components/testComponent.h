#ifndef TEST_COMPONENT_H_
#define TEST_COMPONENT_H_

#include "component.h"

namespace CutePathSim
{
  class TestComponent : public Component
  {
    public:
      class AdderComponent : public Component
      {
        public:
          AdderComponent(const QString &name, QGraphicsItem *parent = 0);
          ~AdderComponent();

          void run();

          QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

        private:
          Input *m_input_01, *m_input_02;
          Output *m_output;
      };

      class MultiplierComponent : public Component
      {
        public:
          MultiplierComponent(const QString &name, QGraphicsItem *parent = 0);
          ~MultiplierComponent();

          void run();

          QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

        private:
          Input *m_input_01, *m_input_02;
          Output *m_output;
      };

      TestComponent(const QString &name, QGraphicsItem *parent = 0);
      ~TestComponent();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Input *m_input_01, *m_input_02, *m_multiplyFlag;
      Output *m_output;
      AdderComponent *m_adder;
      MultiplierComponent *m_multiplier;
  };
}

#endif
