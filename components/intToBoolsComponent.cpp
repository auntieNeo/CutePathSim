#include <QDebug>

#include "components/intToBoolsComponent.h"

namespace CutePathSim
{
  IntToBoolsComponent::IntToBoolsComponent(const QString &name, int width, bool bigEndian, QGraphicsItem *parent) : Component(name, parent)
  {
    m_width = width;
    m_bigEndian = bigEndian;

    addInput(m_input = new Input("input", width, this));

    for(int i = 0; i < width; i++)
    {
      m_outputBools.append(new Output(QVariant(i).toString(), 1, this));
      addOutput(m_outputBools.last());
    }

    setLayout(MINIMIZED);
  }

  IntToBoolsComponent::~IntToBoolsComponent()
  {
  }

  void IntToBoolsComponent::run()
  {
    int inputSize = m_width / 8 + ((m_width % 8) ? 1 : 0);
    unsigned char *input = new unsigned char[inputSize];
    m_input->read(input);
    for(int i = 0; i < inputSize; i++)
    {
      qDebug() << "input[" << i << "]:" << input[i];
    }
    for(int i = 0; i < m_width; i++)
    {
      qDebug() << "output bool" << i << ":" << bool(input[i/8] & (0x01 << (i % 8)));
      m_outputBools[i]->writeBool((input[i/8] & (0x01 << (i % 8))) ? 0x01 : 0x00);
    }
    delete input;
  }
}
