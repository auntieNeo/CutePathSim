#include "arithmeticLogicUnit.h"

namespace CutePathSim
{
  ArithmeticLogicUnit::ArithmeticLogicUnit(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addOutput(m_output = new Output("output", 32, this));
  }

  ArithmeticLogicUnit::~ArithmeticLogicUnit()
  {
    delete m_output;
  }

  void ArithmeticLogicUnit::run()
  {
  }
}
