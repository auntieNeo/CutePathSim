#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <QGraphicsItem>
#include <QMap>
#include <QSet>
#include <QString>

#include "interface.h"

namespace CutePathSim
{
  class ComponentGraph;

  class Component : public QGraphicsItem
  {
    public:
      friend class ComponentGraph;
      class Output;
      class Input : public Interface
      {
        public:
          friend class Output;

          Input(const QString &name, int width, Component *m_component);
          ~Input();

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
          QColor color() const { return QColor(0xA6, 0xD6, 0xA6); }  // light green

        private:
          int m_width, m_bufferSize;
          char *m_inputBuffer;
          Component *m_component;
          Output *m_connection;
      };

      class Output : public Interface
      {
        public:
          friend class Input;

          Output(const QString &name, int width, Component *m_component);
          ~Output();

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
          QColor color() const { return QColor(0xA6, 0xA6, 0xD6); }  // light blue

        private:
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

      QRectF boundingRect() const;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    protected:
      ComponentGraph *m_componentGraph;

      void mousePressEvent(QGraphicsSceneMouseEvent *event);
      void mouseDragEvent(QGraphicsSceneDragDropEvent *event);

      void addInput(Input *input);
      void addOutput(Output *output);

    private:
      // style options for drawing the components
      // FIXME: I don't know why the following code manages to link without these statics being defined somewhere.
      static const qreal BORDER_BRUSH_WIDTH = 2;
      static const qreal LEFT_MARGIN = 20, RIGHT_MARGIN = 20, TOP_MARGIN = 35, BOTTOM_MARGIN = 20, INTERFACE_MARGIN = 7;
      static const qreal FONT_SIZE = 20;

      QMap<QString, Input *> m_inputs;
      QMap<QString, Output *> m_outputs;

      void repositionInterfaces();
      qreal maxInterfaceWidth() const;
  };
}

#endif
