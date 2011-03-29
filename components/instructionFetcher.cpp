#include "instructionFetcher.h"

namespace CutePathSim
{
  InstructionFetcher::InstructionFetcher(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addOutput(m_output = new Output("output", 32, this));
  }

  InstructionFetcher::~InstructionFetcher()
  {
    delete m_output;
  }

  void InstructionFetcher::run()
  {
  }
}
