#ifndef PRINT_INT_COMPONENT_H_
#define PRINT_INT_COMPONENT_H_

#include "component.h"

namespace CutePathSim
{
  class PrintIntComponent : public Component
  {
    public:
      PrintIntComponent(const QString &name, QGraphicsItem *parent = 0);
      ~PrintIntComponent();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Input *m_input;
  };
}

#endif
