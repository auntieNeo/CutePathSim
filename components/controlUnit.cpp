#include "controlUnit.h"

namespace CutePathSim
{
  ControlUnit::ControlUnit(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addInput(m_instruction = new Input("Instruction", 32, this));
    addOutput(m_registerAddress = new Output("RegDst", 1, this));
    addOutput(m_registerAddress = new Output("ALUCtr", 32, this));
  }

  ControlUnit::~ControlUnit()
  {
    delete m_instruction;
  }

  void ControlUnit::run()
  {
  }
}
