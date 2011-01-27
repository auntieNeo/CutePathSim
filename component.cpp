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
}
