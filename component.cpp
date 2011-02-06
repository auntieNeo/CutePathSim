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
  Component::Input::Input(const QString &name, int width, Component *component)
  {
    m_name = name;
    m_width = width;
    m_bufferSize = width / 8 + ((width % 8) ? 1 : 0);
    m_inputBuffer = new char[m_bufferSize];
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
   * \fn Component::Input::component()
   * Returns a pointer to the component this input corresponds to, the recipient of the data.
   */

  /**
   * \fn Component::Input::connection()
   * Returns a pointer to the output that is connected to this input.
   */

  /**
   * Connects this input to \a output. Any existing connection is disconnected, as inputs can only recieve from one output at a time.
   */
  void Component::Input::connect(Output *output)
  {
    disconnect();

    if(output == 0)
      return;

    output->m_connect(this);
    m_connect(output);
  }

  /**
   * Disconnects the input.
   * \fixme I'm not even sure if this should even be implemented. As of now, the graph assumes that each component depends upon each of it's inputs during the topological sort, so for a disconnected input to make any sense, there should be a "dependant" property for each input, and proper handling of unconnected inputs.
   */
  void Component::Input::disconnect()
  {
    if(m_connection)
    {
      m_connection->m_disconnect(this);
      m_disconnect();
    }
  }

  /**
   * \class Component::Output
   * The Output class represents an output interface on a Component.
   */

  /**
   * \fn Component::Output::name()
   * Returns the name of the output.
   */

  /**
   * \fn Component::Output::width()
   * Returns the number of bits that the output sends out.
   */

  /**
   * \fn Component::Output::component()
   * Returns a pointer to the component this output corresponds to, the sender of the data.
   */

  /**
   * Connects this output to \a input.
   */
  void Component::Output::connect(Input *input)
  {
    if(input == 0)
      return;

    m_connect(input);
    input->m_connect(this);
  }

  /**
   * Disconnects \a input from this output.
   */
  void Component::Output::disconnect(Input *input)
  {
    if(m_connections.contains(input))
    {
      input->m_disconnect();
      m_disconnect(input);
    }
  }

  /**
   * Writes \a data to the input buffer of all of the inputs that this output is connected to.
   *
   * This should be called for each output on each cycle, otherwise the receiving input buffers will stay the same as they were the last cycle!
   *
   * Classes calling write() should politely make sure that any remainder bits in \a data (i.e. bits greater than width() if width() % 8 != 0) are zeroed out.
   */
  void Component::Output::write(const char *data)
  {
    foreach(Input *input, m_connections)
    {
      input->writeToInput(data);
    }
  }
}
