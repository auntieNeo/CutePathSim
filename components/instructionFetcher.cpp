#include "assembler.h"
#include "instructionFetcher.h"

namespace CutePathSim
{
  InstructionFetcher::InstructionFetcher(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addOutput(m_output = new Output("output", 32, this));
    m_textEdit = new QPlainTextEdit;
    m_assemtext = 0;
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

      m_assemtext->appendPlainText(hexassembly);

    }

  }

  QToolBox *InstructionFetcher::getToolBox()
  {
    QToolBox *toolBox = Component::getToolBox();
    if(m_assemtext == 0)
    {
        toolBox->addItem(m_assemtext = new QPlainTextEdit(),QObject::tr("Instructions"));
        toolBox->addItem(m_textEdit, QObject::tr("Assembly Code"));
        m_assemtext->setReadOnly(true);
    }


    return toolBox;

  }


  void InstructionFetcher::closeToolBox()
  {
    Component::closeToolBox();
    delete m_assemtext;
    m_assemtext = 0;
  }
}
