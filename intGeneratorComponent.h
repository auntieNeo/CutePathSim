#ifndef INT_GENERATOR_COMPONENT_H_
#define INT_GENERATOR_COMPONENT_H_

#include "component.h"

namespace CutePathSim
{
  class IntGeneratorComponent : public Component
  {
    public:
      IntGeneratorComponent(const QString &name, int integer, QGraphicsItem *parent = 0);
      ~IntGeneratorComponent();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Output *m_output;
      unsigned int m_integer;
  };
}

#endif
