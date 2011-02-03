#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <QGraphicsItem>
#include <QHash>
#include <QMutex>
#include <QMutexLocker>
#include <QString>

namespace CutePathSim
{
  class Component : public QGraphicsItem
  {
    typedef int (Component::*InputCallback) (char *data, int width);
    public:
      class Output;
      class Input
      {
        public:
          Input(const QString &name, int width, char *inputBuffer, Component *m_component);
          ~Input();

          QString name() { return m_name; }
          int width() { return m_width; }
          void writeToInput(char *data);
          void waitInputMutex();
          Component *component() { return m_component; }
          Output *connection() { return m_connection; }

        private:
          QString m_name;
          int m_width;
          char *m_inputBuffer;
          QMutex *m_inputBufferMutex;
          QMutexLocker *m_inputBufferMutexLocker;
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
          Component *component() { return m_component; }
          QList<Input *> connections() { return m_connections; }

        private:
          QString m_name;
          int m_width;
          Component *m_component;
          QList<Input *> m_connections;
      };

      Component(QGraphicsItem *parent = 0);
      ~Component();

      QList<Input *> getInputs() { return m_inputs.values(); }
      QList<Output *> getOutputs() { return m_outputs.values(); }

      Input *getInputs(const QString &name);
      Input *getOutputs(const QString &name);

      virtual void run() = 0;

    protected:
      void mousePressEvent(QGraphicsSceneMouseEvent *event);
      void mouseDragEvent(QGraphicsSceneDragDropEvent *event);

      void addInput(Input *input);
      void addOutput(Output *output);

    private:

      QHash<QString, Input *> m_inputs;
      QHash<QString, Output *> m_outputs;
  };
}

#endif
