#ifndef CONTROL_UNIT_H_
#define CONTROL_UNIT_H_

#include "component.h"

namespace CutePathSim
{
  class ControlUnit : public Component
  {
    public:
      ControlUnit(const QString &name, QGraphicsItem *parent = 0);
      ~ControlUnit();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Input *m_instruction;
      Output *m_aluOp, *m_registerOp, *m_registerAddress;
  };
}

#endif
