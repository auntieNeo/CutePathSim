#ifndef INSTRUCTION_FETCHER_H_
#define INSTRUCTION_FETCHER_H_

#include "component.h"

namespace CutePathSim
{
  class InstructionFetcher : public Component
  {
    public:
      InstructionFetcher(const QString &name, QGraphicsItem *parent = 0);
      ~InstructionFetcher();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Output *m_output;
  };
}

#endif
