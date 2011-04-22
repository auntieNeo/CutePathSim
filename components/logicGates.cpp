#include "logicGates.h"

namespace CutePathSim
{
  AndGate::AndGate(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addInput(m_a = new Input("a", 1, this));
    addInput(m_b = new Input("b", 1, this));
    addOutput(m_output = new Output("output", 1, this));

    setLayout(MINIMIZED);
  }

  AndGate::~AndGate()
  {
  }

  void AndGate::run()
  {
    unsigned char a = m_a->readBool() ? 0x01 : 0x00;
    unsigned char b = m_b->readBool() ? 0x01 : 0x00;
    m_output->writeBool(a & b);
  }

  OrGate::OrGate(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addInput(m_a = new Input("a", 1, this));
    addInput(m_b = new Input("b", 1, this));
    addOutput(m_output = new Output("output", 1, this));

    setLayout(MINIMIZED);
  }

  OrGate::~OrGate()
  {
  }

  void OrGate::run()
  {
    unsigned char a = m_a->readBool() ? 0x01 : 0x00;
    unsigned char b = m_b->readBool() ? 0x01 : 0x00;
    m_output->writeBool(a | b);
  }

  XOrGate::XOrGate(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addInput(m_a = new Input("a", 1, this));
    addInput(m_b = new Input("b", 1, this));
    addOutput(m_output = new Output("output", 1, this));

    setLayout(MINIMIZED);
  }

  XOrGate::~XOrGate()
  {
  }

  void XOrGate::run()
  {
    unsigned char a = m_a->readBool() ? 0x01 : 0x00;
    unsigned char b = m_b->readBool() ? 0x01 : 0x00;
    m_output->writeBool(a ^ b);
  }
}
