#include "registers.h"

namespace CutePathSim
{
  Registers::Registers(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addOutput(m_output = new Output("output", 32, this));
    addInput(m_opCode = new Input("Op Code", 6, this));
  }

  Registers::~Registers()
  {
  }

  void Registers::run()
  {
  }
}
