#include <QDebug>

#include "printIntComponent.h"

namespace CutePathSim
{
  /**
   * \class PrintIntComponent
   * The PrintIntComponent is used as a component debuging tool. It outputs the integer that it recieves on its input to stdout.
   */

  /**
   * Constructs an PrintIntComponent object.
   */
  PrintIntComponent::PrintIntComponent(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    // construct the input and add it to the component's list of inputs
    addInput(m_input = new Input("input", 32, this));
  }

  PrintIntComponent::~PrintIntComponent()
  {
    // it's important to destroy all of the inputs/outputs that we created with the new operator
    delete m_input;
  }

  void PrintIntComponent::run()
  {
    // write the input to stdout
    qDebug() << name() << "received:" << m_input->readInt();
  }
}
