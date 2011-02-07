#include "mux.h"

namespace CutePathSim
{
  /**
   * Constructs a component that represents a mux.
   *
   * \todo document what a mux and its inputs/outputs are.
   */
  Mux::Mux(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    // construct the inputs and add them to the component's list of inputs
    addInput(m_firstInput = new Input("firstInput", 1, 0));
    addInput(m_secondInput = new Input("secondInput", 1, 0));
    // construct the output and add it to the component's list of outputs
    addOutput(m_output = new Output("output", 1, 0));
  }

  Mux::~Mux()
  {
    // it's important to destroy all of the inputs/outputs that we created with the new operator
    delete m_firstInput;
    delete m_secondInput;
    delete m_output;
  }

  void Mux::run()
  {
    // read from the inputs
    m_firstInput->read(&m_firstInputBuffer);
    m_secondInput->read(&m_secondInputBuffer);

    // write to the output
    char result = m_firstInputBuffer ^ m_secondInputBuffer;  // FIXME: I don't know how a mux works, so this is just xor. Someone should implement this properly. :P
    m_output->write(&result);
  }
}
