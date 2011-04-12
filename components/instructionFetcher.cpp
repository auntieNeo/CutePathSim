#include "assembler.h"
#include "instructionFetcher.h"

namespace CutePathSim
{
  InstructionFetcher::InstructionFetcher(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addOutput(m_output = new Output("output", 32, this));
    m_textEdit = new QPlainTextEdit();
  }

  InstructionFetcher::~InstructionFetcher()
  {
    delete m_output;
  }

  void InstructionFetcher::run()
  {
    if(m_textEdit->document()->isModified())
    {
      QString text = m_textEdit->toPlainText();
      QTextStream *textStream = new QTextStream(&text, QIODevice::ReadOnly);
      m_assembled = parseAssembly(*textStream);
      delete textStream;
      m_textEdit->document()->setModified(false);
      m_instructionCounter = 0;
      QByteArray hexassembly = m_assembled.toHex();

      for(i=0; i < hexassembly.length(); i++)
      {
          if (hexassembly.at(i))
          {
              m_assemtext->appendPlainText(hexassembly);
          }
      }

    }

  }

  QToolBox *InstructionFetcher::getToolBox()
  {
    QToolBox *toolBox = Component::getToolBox();
    toolBox->addItem(m_textEdit, QObject::tr("Assembly Code"));
    toolBox->addItem(*m_assemtext, QObject::tr("Instructions"));
    return toolBox;

  }


  void InstructionFetcher::closeToolBox()
  {
    Component::closeToolBox();
  }
}
