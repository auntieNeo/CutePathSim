#include "mux.h"

namespace CutePathSim
{
  /**
   * Constructs a component used for demonstrating and testing the features of Component.
   */
  TestComponent::TestComponent(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    // construct the inputs and add them to the component's list of inputs
    addInput(m_firstInput = new Input("firstInput", 1, this));
    addInput(m_secondInput = new Input("secondInput", 1, this));
    // construct the output and add it to the component's list of outputs
    addOutput(m_output = new Output("output", 1, this));
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
    m_firstInput->read(&m_firstInputBuffer);
    m_secondInput->read(&m_secondInputBuffer);

    // write to the output
    char result = m_firstInputBuffer ^ m_secondInputBuffer;  // FIXME: I don't know how a mux works, so this is just xor. Someone should implement this properly. :P
    m_output->write(&result);
  }
}
