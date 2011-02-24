#ifndef LOGIC_GATES_H_
#define LOGIC_GATES_H_

#include "component.h"

namespace CutePathSim
{
  class AndGate : public Component
  {
    public:
      AndGate(const QString &name, QGraphicsItem *parent = 0);
      ~AndGate();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Input *m_a, *m_b;
      Output *m_output;
  };

  class OrGate : public Component
  {
    public:
      OrGate(const QString &name, QGraphicsItem *parent = 0);
      ~OrGate();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Input *m_a, *m_b;
      Output *m_output;
  };

  class XOrGate : public Component
  {
    public:
      XOrGate(const QString &name, QGraphicsItem *parent = 0);
      ~XOrGate();

      void run;

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Input *m_a, *m_b;
      Output *m_output;
  };
}

#endif
