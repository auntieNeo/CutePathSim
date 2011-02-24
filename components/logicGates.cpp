#include "logicGates.h"

namespace CutePathSim
{
  AndGate::AndGate(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addInput(m_a = new Input("a", 1, this));
    addInput(m_b = new Input("b", 1, this));
    addOutput(m_output = new Output("output", 1, this));
  }

  AndGate::~AndGate()
  {
    delete m_a;
    delete m_b;
    delete m_output;
  }

  void AndGate::run()
  {
    unsigned char a = m_a->readBool() ? 0x01 : 0x00;
    unsigned char b = m_b->readBool() ? 0x01 : 0x00;
    m_output->writeBool(a & b);
  }

  OrGate::OrGate(const QString &name, QGraphicsItem *parent)
  {
    addInput(m_a = new Input("a", 1, this));
    addInput(m_b = new Input("b", 1, this));
    addOutput(m_output = new Output("output", 1, this));
  }

  OrGate::~OrGate()
  {
    delete m_a;
    delete m_b;
    delete m_output;
  }

  void OrGate::run()
  {
    unsigned char a = m_a->readBool() ? 0x01 : 0x00;
    unsigned char b = m_b->readBool() ? 0x01 : 0x00;
    m_output->writeBool(a | b);
  }

  XOrGate::XOrGate(const QString &name, QGraphicsItem *parent)
  {
    addInput(m_a = new Input("a", 1, this));
    addInput(m_b = new Input("b", 1, this));
    addOutput(m_output = new Output("output", 1, this));
  }

  XOrGate::~XOrGate()
  {
    delete m_a;
    delete m_b;
    delete m_output;
  }

  void XOrGate::run()
  {
    unsigned char a = m_a->readBool() ? 0x01 : 0x00;
    unsigned char b = m_b->readBool() ? 0x01 : 0x00;
    m_output->writeBool(a ^ b);
  }
}
