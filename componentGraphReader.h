#ifndef COMPONENT_GRAPH_READER_H_
#define COMPONENT_GRAPH_READER_H_

#include <QXmlStreamWriter>

namespace CutePathSim
{
  class Component;
  class ComponentGraph;

  class ComponentGraphReader
  {
    public:
      ComponentGraphReader(ComponentGraph *componentGraph);
      ~ComponentGraphReader();

      bool readFile(QIODevice *device);

    private:
      QXmlStreamReader xml;
      ComponentGraph *m_componentGraph;
      struct Connection
      {
        Component::Output *output;
        QString componentName;
        QString inputName;
      };

      void readComponentGraph(ComponentGraph *componentGraph);
      QList<Connection> readComponent(ComponentGraph *componentGraph);

      Component *constructComponent(const QString &type, const QString &name);
  };
}

#endif
