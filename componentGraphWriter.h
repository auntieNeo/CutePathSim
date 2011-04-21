#ifndef COMPONENT_GRAPH_WRITER_H_
#define COMPONENT_GRAPH_WRITER_H_

#include <QXmlStreamWriter>

namespace CutePathSim
{
  class Component;
  class ComponentGraph;

  class ComponentGraphWriter
  {
    public:
      ComponentGraphWriter(ComponentGraph *componentGraph);
      ~ComponentGraphWriter();

      bool writeFile(QIODevice *device);

    private:
      QXmlStreamWriter xml;
      ComponentGraph *m_componentGraph;

      void writeComponentGraph(ComponentGraph *componentGraph);
      void writeComponent(Component *component);
  };
}

#endif
