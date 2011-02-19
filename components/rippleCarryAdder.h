#ifndef RIPPLE_CARRY_ADDER_H_
#define RIPPLE_CARRY_ADDER_H_

#include "component.h"

namespace CutePathSim
{
  class RippleCarryAdder : public Component
  {
    public:
      class FullAdder : public Component
      {
        public:
          FullAdder(const QString &name, QGraphicsItem *parent = 0);
          ~FullAdder();

          void run();

          QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

        private:
          Input *m_a, *m_b, *m_cin;
          Output *m_cout, *m_s;
      };

      RippleCarryAdder(const QString &name, int width, QGraphicsItem *parent = 0);
      ~RippleCarryAdder();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Input *m_a, *m_b;
      Output *m_sum;
      QList<FullAdder *> adders;
  };
}

#endif
