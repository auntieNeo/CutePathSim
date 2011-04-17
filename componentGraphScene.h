#ifndef COMPONENT_GRAPH_SCENE_H_
#define COMPONENT_GRAPH_SCENE_H_

#include <QGraphicsScene>

#include "common.h"
#include "component.h"

namespace CutePathSim
{
  class DragInterface;
  class Edge;

  class ComponentGraphScene : public QGraphicsScene
  {
    Q_OBJECT

    friend class Component::Input;
    friend class Component::Output;

    public:
      ComponentGraphScene(QObject *parent = 0);
      ~ComponentGraphScene();

      bool addComponent(Component *component);

      ComponentGraph *rootGraph() { return m_rootGraph; };

    protected:
      void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
      void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
      void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    private:
      ComponentGraph *m_rootGraph;
      Interface *m_selectedInterface, *m_draggedInterface;
      DragInterface *m_dragInterface;
      QPointF m_startDragPoint;
      bool m_moved;
  };

  class DragInterface : public QGraphicsItem
  {
    public:
      DragInterface(Interface *interface, QGraphicsItem *parent = 0);
      ~DragInterface();

      void move(QPointF offset);

      QRectF boundingRect() const;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    private:
      Interface *m_fromInterface;
      QPointF m_offset;
  };
}

#endif
