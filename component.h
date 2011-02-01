#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <QHash>

namespace CutePathSim
{
  class Component : public QGraphicsItem
  {
    typedef int (Component::*InputCallback) (char *data, int width);
    public:
      class Input
      {
        public:
          Input(const QString &name, int width, char *inputBuffer, Component *m_component);
          ~Input();

          QString name() { return m_name; }
          int width() { return m_width; }
          char *inputBuffer() { return m_inputBuffer; }
          void unlockInputMutex();
          void waitInputMutex();
          Component *component() { return m_component; }
          Output *connection() { return m_connection; }

        private:
          QString m_name;
          int m_width;
          char *m_inputBuffer;
          QMutex m_inputBufferMutex;
          QMutexLocker m_inputBufferMutexLocker;
          Component *m_component;
          Output *m_connection;
      };

      class Output
      {
        public:
          Output(const QString &name, int width, Component *m_component);
          ~Output();

          // TODO: Add a connect/disconnect method?
          QString name() { return m_name; }
          int width() { return m_width; }
          Component *component { return m_component; }
          QList<Input *> connections() { return m_connections; }

        private:
          QString m_name;
          int m_width;
          Component *m_component;
          QList<Input *> m_connections;
      };

      Component(QGraphicsItem *parent = 0);
      ~Component();

      QList<Input> getInputs();
      QList<Outputs> getOutputs();

      Input *getInput(const &QString name);
      Input *getOutput(const &QString name);

      virtual void run() = 0;

    protected:
      void mousePressEvent(QGraphicsSceneMouseEvent *event);
      void mouseDragEvent(QGraphicsSceneDragDropEvent *event);

      void addInput(Input *input);
      void addOutput(Output *output);

    private:

      QHash<Input *, QString>;
      QHash<Output *, QString>;
      QList<pointertocomponent pointertomethod
  };
}

#endif
