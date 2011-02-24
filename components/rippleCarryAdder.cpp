#include <QDebug>

#include "boolGeneratorComponent.h"
#include "boolsToIntComponent.h"
#include "componentGraph.h"
#include "intToBoolsComponent.h"
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
    qDebug() << "adder:" << name() << "a:" << bool(a) << "b:" << bool(b) << "cin:" << bool(cin) << "cout:" << bool((a & b) | (cin & (a ^ b))) << "s:" << bool(a ^ b ^ cin);

    m_cout->writeBool((a & b) | (cin & (a ^ b)));
    m_s->writeBool(a ^ b ^ cin);
  }

  RippleCarryAdder::RippleCarryAdder(const QString &name, int width, QGraphicsItem *parent) : Component(name, parent)
  {
    addInput(m_a = new Input("a", width, this));
    addInput(m_b = new Input("b", width, this));
    addOutput(m_sum = new Output("sum", width, this));

    // TODO: feed the first adder a false carry bit
    // make the adders
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

    // feed the first adder a false carry bit
    addSubComponent(m_false = new BoolGeneratorComponent("false", false, this));
    m_false->getOutput("output")->connect(adders[0]->getInput("cin"));

    // convert the ints and bools
    addSubComponent(m_intToBoolsA = new IntToBoolsComponent("IntToBoolsA", width));
    addSubComponent(m_intToBoolsB = new IntToBoolsComponent("IntToBoolsB", width));
    addSubComponent(m_boolsToInt = new BoolsToIntComponent("BoolsToInt", width));
    // connect int/bool conversions to the adders
    m_a->from()->connect(m_intToBoolsA->getInput("input"));
    m_b->from()->connect(m_intToBoolsB->getInput("input"));
    for(int i = 0; i < width; i++)
    {
      m_intToBoolsA->getOutput(QVariant(i).toString())->connect(adders[i]->getInput("a"));
      m_intToBoolsB->getOutput(QVariant(i).toString())->connect(adders[i]->getInput("b"));
      adders[i]->getOutput("s")->connect(m_boolsToInt->getInput(QVariant(i).toString()));
    }
    m_boolsToInt->getOutput("output")->connect(m_sum->to());

    subGraph()->layoutGraph();  // FIXME: this shouldn't be needed in the future
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
    // FIXME: get rid of this manual run code
    m_intToBoolsA->run();
    m_intToBoolsB->run();
    m_false->run();
    foreach(FullAdder *adder, adders)
    {
      adder->run();
    }
    m_boolsToInt->run();
  }
}
