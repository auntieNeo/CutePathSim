#include "boolGeneratorComponent.h"

namespace CutePathSim
{
  /**
   * \class BoolGeneratorComponent
   * The BoolGeneratorComponent is used as a component debuging tool. It outputs the same boolean value every cycle.
   */

  /**
   * Constructs an BoolGeneratorComponent object. \a boolean is the boolean value that the object will output every cycle.
   */
  BoolGeneratorComponent::BoolGeneratorComponent(const QString &name, bool boolean, QGraphicsItem *parent) : Component(name, parent)
  {
    m_boolean = boolean;
    // construct the output and add it to the component's list of outputs
    addOutput(m_output = new Output("output", 1, this));
  }

  BoolGeneratorComponent::~BoolGeneratorComponent()
  {
    // it's important to destroy all of the inputs/outputs that we created with the new operator
    delete m_output;
  }

  void BoolGeneratorComponent::run()
  {
    // write to the output
    m_output->writeBool(m_boolean);
  }
}
