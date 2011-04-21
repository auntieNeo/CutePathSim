#ifndef REGISTERS_H_
#define REGISTERS_H_

#include "component.h"

namespace CutePathSim
{
  class Registers : public Component
  {
    public:
      Registers(const QString &name, QGraphicsItem *parent = 0);
      ~Registers();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Output *m_output;
      Input *m_opCode;
  };
}

#endif
