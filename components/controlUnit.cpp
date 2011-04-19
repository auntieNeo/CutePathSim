#include "controlUnit.h"

namespace CutePathSim
{
  ControlUnit::ControlUnit(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addInput(m_instruction = new Input("Instruction", 32, this));
    addOutput(m_aluOp = new Output("ALU Op", 6, this));
    addOutput(m_registerOp = new Output("Register Op", 6, this));
    addOutput(m_registerAddress = new Output("Register Address", 6, this));
  }

  ControlUnit::~ControlUnit()
  {
    delete m_instruction;
  }

  void ControlUnit::run()
  {
  }
}
