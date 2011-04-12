#ifndef ARITHMETIC_LOGIC_UNIT_H_
#define ARITHMETIC_LOGIC_UNIT_H_

#include "component.h"

namespace CutePathSim
{
  class ArithmeticLogicUnit : public Component
  {
    public:
      ArithmeticLogicUnit(const QString &name, QGraphicsItem *parent = 0);
      ~ArithmeticLogicUnit();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Output *m_output;
  };
}

#endif
