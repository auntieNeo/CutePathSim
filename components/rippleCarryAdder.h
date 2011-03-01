#ifndef RIPPLE_CARRY_ADDER_H_
#define RIPPLE_CARRY_ADDER_H_

#include "component.h"

namespace CutePathSim
{
  class AndGate;
  class BoolGeneratorComponent;
  class BoolsToIntComponent;
  class IntToBoolsComponent;
  class OrGate;
  class XOrGate;

  class RippleCarryAdder : public Component
  {
    public:
      class FullAdder : public Component
      {
        public:
          FullAdder(const QString &name, QGraphicsItem *parent = 0);
          ~FullAdder();

          void run();

          QColor color() { return QColor(0xE6, 0xE6, 0x00); }  // yellow

        private:
          Input *m_a, *m_b, *m_cin;
          Output *m_cout, *m_s;

          AndGate *m_and0, *m_and1;
          XOrGate *m_xor0, *m_xor1;
          OrGate *m_or;
      };

      RippleCarryAdder(const QString &name, int width, QGraphicsItem *parent = 0);
      ~RippleCarryAdder();

      void run();

      QColor color() { return QColor(0xFF, 0xB6, 0x09); }  // light orange

    private:
      Input *m_a, *m_b;
      Output *m_sum;
      QList<FullAdder *> adders;
      BoolGeneratorComponent *m_false;
      IntToBoolsComponent *m_intToBoolsA, *m_intToBoolsB;
      BoolsToIntComponent *m_boolsToInt;
  };
}

#endif
