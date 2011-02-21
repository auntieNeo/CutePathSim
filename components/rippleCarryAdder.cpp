#include "componentGraph.h"
#include "rippleCarryAdder.h"

namespace CutePathSim
{
  RippleCarryAdder::FullAdder::FullAdder(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    addInput(m_a = new Input("a", 1, this));
    addInput(m_b = new Input("b", 1, this));
    addInput(m_cin = new Input("cin", 1, this));
    addOutput(m_cout = new Output("cout", 1, this));
    addOutput(m_s = new Output("s", 1, this));
    setLayout(Component::MINIMIZED);
  }

  RippleCarryAdder::FullAdder::~FullAdder()
  {
    delete m_a;
    delete m_b;
    delete m_cin;
    delete m_cout;
    delete m_s;
  }

  void RippleCarryAdder::FullAdder::run()
  {
    unsigned char a = m_a->readBool() ? 0x01 : 0x00;
    unsigned char b = m_b->readBool() ? 0x01 : 0x00;
    unsigned char cin = m_cin->readBool() ? 0x01 : 0x00;

    m_cout->writeBool((a & b) | (cin & (a ^ b)));
    m_s->writeBool(a ^ b ^ cin);
  }

  RippleCarryAdder::RippleCarryAdder(const QString &name, int width, QGraphicsItem *parent) : Component(name, parent)
  {
    addInput(m_a = new Input("a", width, this));
    addInput(m_b = new Input("b", width, this));
    addOutput(m_sum = new Output("sum", width, this));

    // TODO: feed the first adder a false carry bit
    adders.append(new FullAdder("Adder_" + QVariant(0).toString()));
    addSubComponent(adders.last());
    FullAdder *previousAdder = adders.last();
    for(int i = 1; i < width; i++)
    {
      adders.append(new FullAdder("Adder_" + QVariant(i).toString()));
      addSubComponent(adders.last());
      previousAdder->getOutput("cout")->connect(adders.last()->getInput("cin"));
      previousAdder = adders.last();
    }
    // TODO: combine the output of the adders with some sort of multiplexer

    subGraph()->layoutGraph();  // FIXME: this shouldn't be needed in the future
    setLayout(Component::LABELED);  // FIXME: this shouldn't be needed in the future
  }

  RippleCarryAdder::~RippleCarryAdder()
  {
    foreach(FullAdder *adder, adders)
    {
      delete adder;
    }
  }

  void RippleCarryAdder::run()
  {
  }
}
