#include "instructionFetcher.h"

namespace CutePathSim
{
  InstructionFetcher::InstructionFetcher(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addOutput(m_output = new Output("output", 32, this));
    m_textEdit = 0;
  }

  InstructionFetcher::~InstructionFetcher()
  {
    delete m_output;
  }

  void InstructionFetcher::run()
  {
  }

  QToolBox *InstructionFetcher::getToolBox()
  {
    QToolBox *toolBox = Component::getToolBox();
    if(m_textEdit == 0)
    {
      m_textEdit = new QPlainTextEdit(toolBox);
      toolBox->addItem(m_textEdit, QObject::tr("Assembly Code"));
    }
    return toolBox;
  }

  void InstructionFetcher::closeToolBox()
  {
    Component::closeToolBox();
    m_textEdit = 0;
  }
}
