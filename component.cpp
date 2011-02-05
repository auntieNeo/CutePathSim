#include <QGraphicsSceneDragDropEvent>

#include "board.h"
#include "component.h"

namespace CutePathSim
{
  /**
   * \class Component
   * Models a component of a digital system with inputs and outputs.
   */
  Component::Component(QGraphicsItem *parent) : QGraphicsItem(parent)
  {
  }

  Component::~Component()
  {
  }

  /**
   * \fn getInputs()
   * Returns a list containing pointers to the inputs owned by the component.
   *
   * \sa getInput() getOutputs()
   */

  /**
   * \fn getOutputs()
   * Returns a list containing pointers to the outputs owned by the component.
   *
   * \sa getOutput() getInputs()
   */

  /**
   * Returns a pointer to the input associated with the \a name.
   *
   * \sa getInputs() getOutput()
   */
  Component::Input *Component::getInput(const QString &name)
  {
    return m_inputs.value(name);
  }

  /**
   * Returns a pointer to the output associated with the \a name.
   *
   * \sa getOutputs() getInput()
   */
  Component::Output *Component::getOutput(const QString &name)
  {
    return m_outputs.value(name);
  }

  void Component::mousePressEvent(QGraphicsSceneMouseEvent *)
  {
  }

  void Component::mouseDragEvent(QGraphicsSceneDragDropEvent * /*event*/)
  {
    // TODO: implement dragging of components on the graphics scene
  }

  /**
   * Adds an input to the list of inputs used by the component. Should be called from the derived class for every input it has.
   *
   * Component does \b not assume ownership of \a input. The derived class is responsible for storing inputs and outputs.
   * \sa addOutput()
   */
  void Component::addInput(Component::Input * input)
  {
    m_inputs.insert(input->name(), input);
  }

  /**
   * Adds an output to the list of outputs used by the component.
   *
   * Component does \b not assume ownership of \a output. The derived class is responsible for managing inputs and outputs.
   * \sa addInput()
   */
  void Component::addOutput(Component::Output *output)
  {
    m_outputs.insert(output->name(), output);
  }

  /**
   * \fn run()
   * Pure virtual method that is called from the component's work thread. This is where derived classes should do their processing.
   */

  /**
   * \class Component::Input
   * The Input class represents an input interface on a Component.
   */

  /**
   * Constructs an Input class with the name \a name. 
   *
   * The constructed object represents an input that accepts \a width bits and uses the buffer \a inputBuffer to recieve input, with the object \a component as the recipient.
   */
  Component::Input::Input(const QString &name, int width, char *inputBuffer, Component *component)
  {
    m_name = name;
    m_width = width;
    m_inputBuffer = new char[width / 8 + ((width % 8) ? 1 : 0)];
    m_component = component;
  }

  Component::Input::~Input()
  {
    // TODO: Maybe remove this from m_connection's own list of connections? To avoid any nasty segfaults if m_connection tries to connect and dereference us.
  }

  /**
   * \fn Component::Input::name()
   * Returns the name of the input.
   */

  /**
   * \fn Component::Input::width()
   * Returns the number of bits that the input accepts.
   */

  /**
   * Writes to the input buffer for this input.
   *
   * This should be called for each input on each cycle, otherwise the input buffer won't change for that cycle!
   */
  void Component::Input::writeToInput(const char *data)
  {
    memcpy(m_inputBuffer, data, m_width);
  }

  /**
   * \fn Component::Input::component()
   * Returns a pointer to the component this input corresponds to, the recipient of the input.
   */

  /**
   * \fn Component::Input::connection()
   * Returns a pointer to the output that is connected to this input.
   */

  /**
   * \class Component::Output
   * The Output class represents an output interface on a Component.
   */

  /**
   * \fn Component::Input::name()
   * Returns the name of the output.
   */
}
