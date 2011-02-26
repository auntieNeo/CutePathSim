#ifndef BINARY_MULTIPLIER_H_
#define BINARY_MULTIPLIER_H_

#include "component.h"

namespace CutePathSim
{
  class IntToBoolsComponent;
  class RippleCarryAdder;

  class BinaryMultiplier : public Component
  {
    public:
      class BinaryAnder : public Component
      {
        public:
          BinaryAnder(const QString &name, int width, QGraphicsItem *parent = 0);
          ~BinaryAnder();

          void run();

          QColor color() { return QColor(0xFF, 0x2D, 0x0B); }  // light red

        private:
          Input *m_a, *m_b;
          Output *m_output;
      };

      class BinaryShifter : public Component
      {
        public:
          BinaryShifter(const QString &name, int width, int shift, bool bigEndian = false, QGraphicsItem *parent = 0);
          ~BinaryShifter();

          void run();

          QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

        private:
          int m_shift;
          bool m_bigEndian;

          Input *m_input;
          Output *m_output;
      };

      BinaryMultiplier(const QString &name, int width, QGraphicsItem *parent = 0);
      ~BinaryMultiplier();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    private:
      Input *m_a, *m_b;
      Output *m_product;

      IntToBoolsComponent *m_bToBools;
      QList<BinaryAnder *> m_anders;
      QList<BinaryShifter *> m_shifters;
      QList<RippleCarryAdder *> m_adders;
  };
}

#endif
