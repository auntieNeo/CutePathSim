#include "binaryMultiplier.h"
#include "componentGraph.h"
#include "intToBoolsComponent.h"
#include "rippleCarryAdder.h"

namespace CutePathSim
{
  BinaryMultiplier::BinaryAnder::BinaryAnder(const QString &name, int width, QGraphicsItem *parent) : Component(name, parent)
  {
    addInput(m_a = new Input("a", width, this));
    addInput(m_b = new Input("b", 1, this));
    addOutput(m_output = new Output("output", width, this));

    setLayout(MINIMIZED);
  }

  BinaryMultiplier::BinaryAnder::~BinaryAnder()
  {
    delete m_a;
    delete m_b;
    delete m_output;
  }

  void BinaryMultiplier::BinaryAnder::run()
  {
    int bufferSize = m_a->width() / 8 + ((m_a->width() % 8) ? 1 : 0);
    if(m_b->readBool())
    {
      unsigned char *input = new unsigned char[bufferSize];
      m_a->read(input);
      m_output->write(input);
      delete input;
    }
    else
    {
      unsigned char *result = new unsigned char[bufferSize];
      memset(result, 0, bufferSize);
      m_output->write(result);
      delete result;
    }
  }


  BinaryMultiplier::BinaryShifter::BinaryShifter(const QString &name, int width, int shift, bool bigEndian, QGraphicsItem *parent) : Component(name, parent)
  {
    m_shift = shift;
    m_bigEndian = bigEndian;

    addInput(m_input = new Input("input", width, this));
    addOutput(m_output = new Output("output", width, this));

    setLayout(MINIMIZED);
  }

  BinaryMultiplier::BinaryShifter::~BinaryShifter()
  {
    delete m_input;
    delete m_output;
  }

  void BinaryMultiplier::BinaryShifter::run()
  {
    // FIXME: this assumes it's 8 bit for now, because the generalized code would be a bit confusing
//    bufferSize = m_a->width() / 8 + ((m_a->width() % 8) ? 1 : 0);
    Q_ASSERT(m_input->width() == 8);
    unsigned char input;
    m_input->read(&input);
    m_output->write(&(input <<= m_shift));
  }

  BinaryMultiplier::BinaryMultiplier(const QString &name, int width, QGraphicsItem *parent) : Component(name, parent)
  {
    addInput(m_a = new Input("a", width, this));
    addInput(m_b = new Input("b", width, this));
    addOutput(m_product = new Output("product", width, this));

    addSubComponent(m_bToBools = new IntToBoolsComponent("bToBools", width));
    m_b->from()->connect(m_bToBools->getInput("input"));
    for(int i = 0; i < width; i++)
    {
      // make an ander
      m_anders.append(new BinaryAnder("ander" + QVariant(i).toString(), width));
      addSubComponent(m_anders.last());
      // connect the ander to a and bit i of b, effectively "multiplying" a by b[i]
      m_a->from()->connect(m_anders.last()->getInput("a"));
      m_bToBools->getOutput(QVariant(i).toString())->connect(m_anders.last()->getInput("b"));
      // make a bit shifter that shifts by i
      m_shifters.append(new BinaryShifter("shifter" + QVariant(i).toString(), width, i));
      addSubComponent(m_shifters.last());
      // connect the shifter to shift the result of the ander
      m_anders.last()->getOutput("output")->connect(m_shifters.last()->getInput("input"));
      if(i > 0)
      {
        // make an adder
        m_adders.append(new RippleCarryAdder("adder" + QVariant(i).toString(), width));
        addSubComponent(m_adders.last());
        if(i == 1)
        {
          m_adders.last()->setLayout(Component::EXPANDED);
          // connect the adder to add the first two bitshifted results together
          m_shifters[0]->getOutput("output")->connect(m_adders.last()->getInput("a"));
          m_shifters[1]->getOutput("output")->connect(m_adders.last()->getInput("b"));
        }
        else
        {
          m_adders.last()->setLayout(Component::MINIMIZED);
          // connect the previous adder and the new bitshifted result to the new adder to add the result to the total
          m_adders[i - 2]->getOutput("sum")->connect(m_adders.last()->getInput("a"));
          m_shifters.last()->getOutput("output")->connect(m_adders.last()->getInput("b"));
        }
      }
    }
    // connect the last adder to output the product
    m_adders.last()->getOutput("sum")->connect(m_product->to());
  }

  BinaryMultiplier::~BinaryMultiplier()
  {
    delete m_a;
    delete m_b;
    delete m_product;
  }

  void BinaryMultiplier::run()
  {
    /*
    // FIXME: this manual run code won't be needed in the future
    m_bToBools->run();
    foreach(Component *c, m_anders)
    {
      c->run();
    }
    foreach(Component *c, m_shifters)
    {
      c->run();
    }
    foreach(Component *c, m_adders)
    {
      c->run();
    }
    */
    subGraph()->run();
  }
}
