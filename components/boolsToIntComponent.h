#ifndef BOOLS_TO_INT_COMPONENT_H_
#define BOOLS_TO_INT_COMPONENT_H_

#include "component.h"
namespace CutePathSim
{
  class BoolsToIntComponent : public Component
  {
    public:
      BoolsToIntComponent(const QString &name, int width, bool bigEndian = false, QGraphicsItem *parent = 0);
      ~BoolsToIntComponent();

      bool isBigEndian() { return m_bigEndian; }

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

      void run();

    private:
      int m_width;
      bool m_bigEndian;

      QList<Input *> m_inputBools;
      Output *m_output;
  };
}

#endif
