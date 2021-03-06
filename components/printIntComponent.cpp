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
  }

  void PrintIntComponent::run()
  {
    // write the input to stdout
    qDebug() << name() << "received:" << (0x000000FF & m_input->readInt());  // FIXME: There should be a PrintCharComponent or something... This is bad and you should feel bad.
  }
}
