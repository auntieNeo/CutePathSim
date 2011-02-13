#include "componentGraph.h"
#include "testComponent.h"

namespace CutePathSim
{
  /**
   * \class TestComponent
   * TestComponent is a component used for demonstrating and testing the features of Component.
   *
   * This component happens to add if the multiply flag is false, and multiply if the multiply flag is true.
   */

  /**
   * Constructs a TestComponent.
   */
  TestComponent::TestComponent(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    // construct the inputs and add them to the component's list of inputs
    addInput(m_input_01 = new Input("input_01", 32, this));
    addInput(m_input_02 = new Input("input_02", 32, this));
    addInput(m_multiplyFlag = new Input("multiplyFlag", 1, this));
    // construct the output and add it to the component's list of outputs
    addOutput(m_output = new Output("output", 32, this));

    // add the sub-components that will do our computation work
    addSubComponent(m_adder = new AdderComponent("adder"));
    addSubComponent(m_multiplier = new MultiplierComponent("multiplier"));
    m_adder->getOutput("output")->connect(m_multiplier->getInput("input_01"));

    subGraph()->layoutGraph();  // FIXME: this shouldn't be needed in the future
    setLayout(Component::EXPANDED);  // FIXME: this shouldn't be needed in the future
  }

  TestComponent::~TestComponent()
  {
    // it's important to destroy all of the inputs/outputs that we created with the new operator
    delete m_input_01;
    delete m_input_02;
    delete m_multiplyFlag;
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

  /**
   * \class TestComponent::AdderComponent
   * A test component that simply adds its two inputs and outputs the result.
   */
  /**
   * Constructs an adder.
   */
  TestComponent::AdderComponent::AdderComponent(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    // construct the inputs and add them to the component's list of inputs
    addInput(m_input_01 = new Input("input_01", 32, this));
    addInput(m_input_02 = new Input("input_02", 32, this));
    // construct the output and add it to the component's list of outputs
    addOutput(m_output = new Output("output", 32, this));
  }

  TestComponent::AdderComponent::~AdderComponent()
  {
    // it's important to destroy all of the inputs/outputs that we created with the new operator
    delete m_input_01;
    delete m_input_02;
    delete m_output;
  }

  void TestComponent::AdderComponent::run()
  {
    // read from the inputs
    unsigned int first = m_input_01->readInt();
    unsigned int second = m_input_02->readInt();

    // write to the output
    m_output->writeInt(first + second);
  }

  /**
   * \class TestComponent::MultiplierComponent
   * A test component that simply multiplies its two inputs and outputs the result.
   */
  /**
   * Constructs an multiplier.
   */
  TestComponent::MultiplierComponent::MultiplierComponent(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    // construct the inputs and add them to the component's list of inputs
    addInput(m_input_01 = new Input("input_01", 32, this));
    addInput(m_input_02 = new Input("input_02", 32, this));
    // construct the output and add it to the component's list of outputs
    addOutput(m_output = new Output("output", 32, this));
  }

  TestComponent::MultiplierComponent::~MultiplierComponent()
  {
    // it's important to destroy all of the inputs/outputs that we created with the new operator
    delete m_input_01;
    delete m_input_02;
    delete m_output;
  }

  void TestComponent::MultiplierComponent::run()
  {
    // read from the inputs
    unsigned int first = m_input_01->readInt();
    unsigned int second = m_input_02->readInt();

    // write to the output
    m_output->writeInt(first * second);
  }
}
