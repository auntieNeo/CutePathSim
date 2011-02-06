#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <QGraphicsItem>
#include <QHash>
#include <QSet>
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
          friend class Output;

          Input(const QString &name, int width, Component *m_component);
          ~Input();

          QString name() { return m_name; }
          int width() { return m_width; }
          Component *component() { return m_component; }
          Output *connection() { return m_connection; }
          void connect(Output *output);
          void disconnect();
          void read(char *buffer);
          int bufferSize() { return m_bufferSize; }

        protected:
          // to avoid recursions with Output::disconnect()
          void m_disconnect() { m_connection = 0; }
          // to avoid recursions with Output::connect()
          void m_connect(Output *output) { m_connection = output; }
          void writeToInput(const char *data) { memcpy(m_inputBuffer, data, m_bufferSize); }

        private:
          QString m_name;
          int m_width, m_bufferSize;
          char *m_inputBuffer;
          Component *m_component;
          Output *m_connection;
      };

      class Output
      {
        public:
          friend class Input;

          Output(const QString &name, int width, Component *m_component);
          ~Output();

          QString name() { return m_name; }
          int width() { return m_width; }
          Component *component() { return m_component; }
          QSet<Input *> connections() { return m_connections; }
          void connect(Input *input);
          void disconnect(Input *input);
          void write(const char *data);
          int bufferSize() { return m_bufferSize; }

        protected:
          // to avoid recursions with Input::connect()
          void m_connect(Input *input) { m_connections.insert(input); }
          // to avoid recursions with Input::disconnect()
          void m_disconnect(Input *input) { m_connections.remove(input); }

        private:
          QString m_name;
          int m_width, m_bufferSize;
          Component *m_component;
          QSet<Input *> m_connections;
      };

      Component(QGraphicsItem *parent = 0);
      virtual ~Component();

      QList<Input *> getInputs() { return m_inputs.values(); }
      QList<Output *> getOutputs() { return m_outputs.values(); }

      Input *getInput(const QString &name);
      Output *getOutput(const QString &name);

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
