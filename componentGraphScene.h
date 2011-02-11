#ifndef COMPONENT_GRAPH_SCENE_H_
#define COMPONENT_GRAPH_SCENE_H_

#include <QGraphicsScene>

#include "component.h"

namespace CutePathSim
{
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

      void layoutGraph();

    private:
      ComponentGraph *m_rootGraph;
  };
}

#endif
