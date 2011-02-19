#ifndef BOOL_GENERATOR_COMPONENT_H_
#define BOOL_GENERATOR_COMPONENT_H_

#include "component.h"

namespace CutePathSim
{
  class BoolGeneratorComponent : public Component
  {
    public:
      BoolGeneratorComponent(const QString &name, bool boolean, QGraphicsItem *parent = 0);
      ~BoolGeneratorComponent();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Output *m_output;
      unsigned int m_boolean;
  };
}

#endif
