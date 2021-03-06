#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <QGraphicsItem>
#include <QMap>
#include <QSet>
#include <QString>
#include <QToolBox>
#include <QTableView>

#include "common.h"
#include "interface.h"
#include "componentDataModel.h"

namespace CutePathSim
{
  class ComponentGraph;

  class Component : public QGraphicsItem
  {
    public:
      friend class ComponentGraph;
      friend class ComponentDataModel;
      class Output;
      class Input : public Interface
      {
        public:
          friend class Output;

          Input(const QString &name, int width, Component *component = 0);
          virtual ~Input();

          enum { Type = InputType };

          int width() { return m_width; }
          Component *component() const { return m_component; }
          Output *connection() { return m_connection; }
          void connect(Output *output);
          void disconnect();
          void read(unsigned char *buffer);
          bool readBool()
          {
            Q_ASSERT(width() == 1);
            return bool(*m_inputBuffer);  // FIXME: check if this code only uses the first byte, or if it reads four bytes
          }
          unsigned int readInt(bool bigEndian = false);
          int bufferSize() { return m_bufferSize; }
          Output *from();
          bool internal() const { return m_internal; }
          ComponentGraph *parentGraph() const;

        protected:
          // to avoid recursions with Output::disconnect()
          void m_disconnect() { m_connection = 0; }
          // to avoid recursions with Output::connect()
          void m_connect(Output *output) { m_connection = output; }
          void writeToInput(const unsigned char *data);
          QColor color() const { return QColor(0xA6, 0xD6, 0xA6); }  // light green
          int type() const { return Type; }

        private:
          int m_width, m_bufferSize;
          unsigned char *m_inputBuffer;
          Component *m_component;
          Output *m_connection;
          Output *m_from;
          bool m_internal;
          Output *m_externalOutput;
      };

      class Output : public Interface
      {
        public:
          friend class Input;

          Output(const QString &name, int width, Component *component = 0);
          virtual ~Output();

          enum { Type = OutputType };

          int width() { return m_width; }
          Component *component() const { return m_component; }
          QSet<Input *> connections() { return m_connections; }
          void connect(Input *input);
          void disconnect(Input *input);
          void write(const unsigned char *data);
          void writeBool(bool boolean);
          void writeInt(unsigned int integer, bool bigEndian = false);
          int bufferSize() { return m_bufferSize; }
          Input *to();
          bool internal() const { return m_internal; }
          ComponentGraph *parentGraph() const;

        protected:
          // to avoid recursions with Input::connect()
          void m_connect(Input *input) { m_connections.insert(input); }
          // to avoid recursions with Input::disconnect()
          void m_disconnect(Input *input) { m_connections.remove(input); }
          QColor color() const { return QColor(0xA6, 0xA6, 0xD6); }  // light blue
          int type() const { return Type; }

        private:
          int m_width, m_bufferSize;
          Component *m_component;
          QSet<Input *> m_connections;
          Input *m_to;
          bool m_internal;
      };

      enum Layout { NONE = 0, MINIMIZED, LABELED, EXPANDED };

      Component(const QString &name, QGraphicsItem *parent = 0);
      virtual ~Component();

      QString name() { return m_name; }

      QList<Input *> getInputs() const { return m_inputs.values(); }
      QList<Output *> getOutputs() const { return m_outputs.values(); }

      Input *getInput(const QString &name);
      Output *getOutput(const QString &name);

      virtual void run() = 0;

      virtual QColor color() = 0;

      QRectF boundingRect() const;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

      ComponentGraph *parentGraph() const { return m_parentGraph; }
      ComponentGraph *subGraph();

      Layout layout() { return m_layout; }
      void setLayout(Layout layout);

      virtual QToolBox *getToolBox();
      virtual void closeToolBox();

      void addInput(Input *input);
      void addOutput(Output *output);

      Input *removeInput(const QString &name);
      Output *removeOutput(const QString &name);

    protected:
      void setParentGraph(ComponentGraph *graph) { m_parentGraph = graph; }

      void mouseDragEvent(QGraphicsSceneDragDropEvent *event);

      void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
      void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

      void mousePressEvent(QGraphicsSceneMouseEvent *event);
      void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

      void addSubComponent(Component *component);

      void repositionInterfaces();

      void prepareGeometryChange();

      void setData(const QString &name, ComponentData data);
      ComponentData getData(const QString &name);

    private:
      // style options for drawing the components
      // FIXME: I don't know why the following code manages to link without these statics being defined somewhere.
      static const qreal BORDER_PEN_WIDTH = 2;
      static const qreal LEFT_MARGIN = 20, RIGHT_MARGIN = 20, TOP_MARGIN = 50, BOTTOM_MARGIN = 20, INTERFACE_MARGIN = 7, MIN_GRAPH_SIZE = 500;
      static const qreal FONT_SIZE = 20;
      static const qreal EXPAND_BUTTON_SIZE = 30;
      static QFont *m_font;

      QString m_name;
      qreal m_textWidth;

      QMap<QString, Input *> m_inputs;
      QMap<QString, Output *> m_outputs;

      ComponentGraph *m_parentGraph;
      ComponentGraph *m_subGraph;

      Layout m_layout;

      QToolBox *m_toolBox;
      QTableView *m_dataTable;

      qreal maxInterfaceWidth() const;
      qreal maxInterfaceHeight() const;

      QMap<QString, ComponentData> m_data;

      bool m_hovered;
  };
}

#endif
