#include <QGraphicsSceneDragDropEvent>

#include "board.h"
#include "component.h"

namespace CutePathSim
{
  Component::Component(int x, int y, QGraphicsItem *parent) : QGraphicsSvgItem(parent)
  {
  }

  Component::~Component()
  {
  }

  QList<Input *> getInputs();
  QList<Outputs *> getOutputs();

  void Component::mousePressEvent(QGraphicsSceneMouseEvent *)
  {
  }

  void Component::mouseDragEvent(QGraphicsSceneDragDropEvent *event)
  {
    // TODO: implement dragging of components on the graphics scene
  }

  void Component::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
  }

  void Component::mouseDragEvent(QGraphicsSceneDragDropEvent *event)
  {
  }

  void Component::addInput(Input *input, const QString &name)
  {
  }

  void Component::addOutput(Output *output, const QString &name)
  {
  }

  /**
   * \class Component::Input
   * The Input class represents an input interface on a Component.
   */

  /**
   * Constructs an Input class with the name \i name. 
   *
   * The constructed object represents an input that accepts \i width bits and uses the class method \i inputCallback to recieve input, with the object \i component as the recipient.
   */
  Component::Input::Input(const QString &name, int width, InputCallback inputCallback, Component *component)
  {
    m_name = name;
    m_width = width;
    m_inputCallback = inputCallback;
    m_component = component;
  }

  Component::Input::~Input()
  {
    // TODO: Maybe remove this from m_connection's own list of connections? To avoid any nasty segfaults if m_connection tries to connect and dereference us.
  }

  /**
   * \function Component::Input::name()
   * Returns the name of the input.
   */

  /**
   * \function Component::Input::width()
   * Returns the number of bits that the input accepts.
   */

  /**
   * \function Component::Input::inputBuffer()
   * Returns the buffer to be filled when the input recieves data.
   * \note This is the kind of thing that needs a mutex if we're going to have this threaded! Just a plain char* is scarey anyway. I just thought simpler might be better in this case, since we are modeling raw bits. Enough Q_ASSERTS and there shouldn't be any segfaults. *crosses fingers*
   */

  /**
   * Unlocks the mutex placed on the input buffer, basically telling the recipient's thread that it's now okay to read the input buffer.
   * This should be called from the output thread after the input buffer has been filled.
   */
  void Component::Input::unlockInputMutex()
  {
    // TODO
  }

  /**
   * Waits for (and then locks) the mutex, allowing the recipiant thread to read the input buffer.
   * FIXME: There may or may not be a need for another mutex to prevent the input buffer from being changed after it's locked. I'm not certain how the control component is supposed to work, or if it's going to synchronise the threads or something. Some of this threaded stuff is beyond me, but I write it anyway. :P
   * \note These mutexes are the reason that, even though this is threaded, the threads will never give us any real performance benefit, and will probably only hurt us. This is a common pitfall with threads; if there is a lot of message passing between objects, then the problem probably isn't very parallel, and there will be a lot of threads waiting on threads. The reason I decided to go with threads was because it should avoid dealing with components waiting on inputs from components that haven't been called yet, and potentially save me some graph theory.
   */
  void Component::Input::waitInputMutex()
  {
    // TODO
  }

  /**
   * \function Component::Input::component()
   * Returns a pointer to the component this input corresponds to, the recipient of the input.
   */

  /**
   * \function Component::Input::connection()
   * Returns a pointer to the output that is connected to this input.
   */
}
