#ifndef MUX_H_
#define MUX_H_

#include "component.h"

namespace CutePathSim
{
  class Mux : public Component
  {
    public:
      Mux(QGraphicsItem *parent = 0);
      ~Mux();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Input *m_firstInput, *m_secondInput;
      Output *m_output;
      char m_firstInputBuffer, m_secondInputBuffer;
  };
}

#endif
