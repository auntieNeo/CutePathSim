#ifndef INT_TO_BOOLS_COMPONENT_H_
#define INT_TO_BOOLS_COMPONENT_H_

#include "component.h"
namespace CutePathSim
{
  class IntToBoolsComponent : public Component
  {
    public:
      IntToBoolsComponent(const QString &name, int width, bool bigEndian = false, QGraphicsItem *parent = 0);
      ~IntToBoolsComponent();

      bool isBigEndian() { return m_bigEndian; }

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

      void run();

    private:
      int m_width;
      bool m_bigEndian;

      Input *m_input;
      QList<Output *> m_outputBools;
  };
}

#endif
