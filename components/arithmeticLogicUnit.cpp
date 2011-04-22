#include "arithmeticLogicUnit.h"

#include <QDebug>

namespace CutePathSim
{
  ArithmeticLogicUnit::ArithmeticLogicUnit(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addOutput(m_output = new Output("output", 32, this));
    addInput(m_opCode = new Input("Op Code", 6, this));
  }

  ArithmeticLogicUnit::~ArithmeticLogicUnit()
  {
  }

  void ArithmeticLogicUnit::run()
  {
    qDebug() << "ALU was run";
  }
}
