#include "intGeneratorComponent.h"

namespace CutePathSim
{
  /**
   * \class IntGeneratorComponent
   * The IntGeneratorComponent is used as a component debuging tool. It outputs the same integer value every cycle.
   */

  /**
   * Constructs an IntGeneratorComponent object. \a integer is the integer value that the object will output every cycle.
   */
  IntGeneratorComponent::IntGeneratorComponent(const QString &name, int integer, QGraphicsItem *parent) : Component(name, parent)
  {
    m_integer = integer;
    // construct the output and add it to the component's list of outputs
    addOutput(m_output = new Output("output", 32, this));
  }

  IntGeneratorComponent::~IntGeneratorComponent()
  {
    // it's important to destroy all of the inputs/outputs that we created with the new operator
    delete m_output;
  }

  void IntGeneratorComponent::run()
  {
    // write to the output
    m_output->writeInt(m_integer);
  }
}

