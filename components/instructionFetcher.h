#ifndef INSTRUCTION_FETCHER_H_
#define INSTRUCTION_FETCHER_H_

#include <QPlainTextEdit>

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

      QToolBox *getToolBox();
      void closeToolBox();

    private:
      Output *m_output;
      QPlainTextEdit *m_textEdit;
  };
}

#endif
