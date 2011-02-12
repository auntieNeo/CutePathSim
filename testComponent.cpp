#include "testComponent.h"

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
    addInput(m_input_01 = new Input("input_01", 32, this));
    addInput(m_input_02 = new Input("input_02", 32, this));
    addInput(m_multiplyFlag = new Input("multiplyFlag", 1, this));
    // construct the output and add it to the component's list of outputs
    addOutput(m_output = new Output("output", 32, this));
  }

  TestComponent::~TestComponent()
  {
    // it's important to destroy all of the inputs/outputs that we created with the new operator
    delete m_input_01;
    delete m_input_02;
    delete m_output;
  }

  void TestComponent::run()
  {
    // read from the inputs
    unsigned int first = m_input_01->readInt();
    unsigned int second = m_input_02->readInt();
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
