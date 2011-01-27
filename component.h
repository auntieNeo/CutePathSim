#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <QHash>

namespace CutePathSim
{
  class Component : public QGraphicsItem
  {
    public:
      Component(QGraphicsItem *parent = 0);
      ~Component();

      QList<Input *> getInputs();
      QList<Outputs *> getOutputs();

    protected:
      void mousePressEvent(QGraphicsSceneMouseEvent *event);
      void mouseDragEvent(QGraphicsSceneDragDropEvent *event);

      void addInput(Input *input, const QString &name);
      void addOutput(Output *output, const QString &name);

    private:
      QHash<Input *, QString>;
      QHash<Output *, QString>;
  };
}

#endif
