#include "controlUnit.h"

namespace CutePathSim
{
  ControlUnit::ControlUnit(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addInput(m_instruction = new Input("instruction", 32, this));
    addOutput(m_opCode = new Output("Op Code", 6, this));
  }

  ControlUnit::~ControlUnit()
  {
    delete m_instruction;
  }

  void ControlUnit::run()
  {
  }
}
