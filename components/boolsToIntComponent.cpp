#include "components/boolsToIntComponent.h"

namespace CutePathSim
{
  BoolsToIntComponent::BoolsToIntComponent(const QString &name, int width, bool bigEndian, QGraphicsItem *parent) : Component(name, parent)
  {
    m_width = width;
    m_bigEndian = bigEndian;

    for(int i = 0; i < width; i++)
    {
      m_inputBools.append(new Input(QVariant(i).toString(), 1, this));
      addInput(m_inputBools.last());
    }

    addOutput(m_output = new Output("output", width, this));
    setLayout(MINIMIZED);
  }

  BoolsToIntComponent::~BoolsToIntComponent()
  {
    foreach(Input *input, m_inputBools)
    {
      delete input;
    }
    delete m_output;
  }

  void BoolsToIntComponent::run()
  {
    int resultSize = m_width / 8 + ((m_width % 8) ? 1 : 0);
    unsigned char *result = new unsigned char[resultSize];
    memset(result, 0, resultSize);
    for(int i = 0; i < resultSize; i++)
    {
      for(int j = 0; j < 8; j++)
      {
        if(m_inputBools[i * j]->readBool())
        {
          result[i] |= 0x01 < j;
        }
      }
    }
    m_output->write(result);
    delete result;
  }
}
