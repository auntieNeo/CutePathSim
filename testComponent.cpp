#include "mux.h"

namespace CutePathSim
{
  /**
   * Constructs a component used for demonstrating and testing the features of Component.
   *
   * This component happens to add if the multiply flag is false, and multiply if multiplyFlag is true.
   */
  TestComponent::TestComponent(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    // construct the inputs and add them to the component's list of inputs
    addInput(m_firstInput = new Input("firstInput", 32, this));
    addInput(m_secondInput = new Input("secondInput", 32, this));
    addInput(m_multiplyFlag = new Input("multiplyFlag", 1, this));
    // construct the output and add it to the component's list of outputs
    addOutput(m_output = new Output("output", 32, this));
  }

  TestComponent::~TestComponent()
  {
    // it's important to destroy all of the inputs/outputs that we created with the new operator
    delete m_firstInput;
    delete m_secondInput;
    delete m_output;
  }

  void TestComponent::run()
  {
    // read from the inputs
    unsigned int first = m_firstInput->readInt();
    unsigned int second = m_secondInput->readInt();
    bool multiplyFlag = m_multiplyFlag->readBool();

    // write to the output
    if(multiplyFlag)
    {
      m_output->writeInt(first * second);
    }
    else
    {
      m_output->writeInt(first + second);
    }
  }
}
