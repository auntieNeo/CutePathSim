#include "component.h"
#include "componentGraph.h"
#include "componentGraphReader.h"
#include "components/luaComponent.h"

#include <QDebug>

namespace CutePathSim
{
  ComponentGraphReader::ComponentGraphReader(ComponentGraph *componentGraph)
  {
    m_componentGraph = componentGraph;
  }

  ComponentGraphReader::~ComponentGraphReader()
  {
  }

  bool ComponentGraphReader::readFile(QIODevice *device)
  {
    xml.setDevice(device);

    if(xml.readNextStartElement())
    {
      qDebug() << "started reading xml";
      if (xml.name() == "component_graph")
      {
        readComponentGraph(m_componentGraph);
      }
      else
      {
        xml.raiseError(QObject::tr("The file is not a valid CutePathSim file."));
      }
    }

    return !xml.error();
  }

  void ComponentGraphReader::readComponentGraph(ComponentGraph *componentGraph)
  {
    qDebug() << "reading a component graph";
    Q_ASSERT(xml.isStartElement() && xml.name() == "component_graph");

    QList<Connection> connections;
    if(xml.readNextStartElement())
    {
      if (xml.name() == "component")
      {
        connections.append(readComponent(componentGraph));
      }
      else
      {
        xml.skipCurrentElement();
      }
    }

    // connect everything now that all the nodes have been added to the graph
    foreach(Connection connection, connections)
    {
      Component *component = componentGraph->getComponent(connection.componentName);
      if(component == 0)
      {
        xml.raiseError(QObject::tr("Parse error: tried to connect to non-existent component."));
        return;
      }
      Component::Input *input = component->getInput(connection.inputName);
      if(input == 0)
      {
        xml.raiseError(QObject::tr("Parse error: tried to connect to non-existent input."));
        return;
      }
      connection.output->connect(input);
    }
  }

  QList<ComponentGraphReader::Connection> ComponentGraphReader::readComponent(ComponentGraph *componentGraph)
  {
    qDebug() << "reading a component";
    Q_ASSERT(xml.isStartElement() && xml.name() == "component");

    QList<Connection> connections;

    Component *component;
    componentGraph->addComponent(component = constructComponent(xml.attributes().value("type").toString(), xml.attributes().value("name").toString()));
    if(xml.readNextStartElement())
    {
      if (xml.name() == "input")
      {
        component->addInput(new Component::Input(xml.attributes().value("name").toString(), QVariant(xml.attributes().value("width").toString()).toInt(), component));
      }
      else if (xml.name() == "output")
      {
        Component::Output *output;
        component->addOutput(output = new Component::Output(xml.attributes().value("name").toString(), QVariant(xml.attributes().value("width").toString()).toInt(), component));
        if(xml.readNextStartElement())
        {
          if(xml.name() == "connection")
          {
            Connection connection;
            connection.output = output;
            connection.componentName = xml.attributes().value("component").toString();
            connection.inputName = xml.attributes().value("input").toString();
            connections.append(connection);
          }
          else
          {
            xml.skipCurrentElement();
          }
        }
      }
      else
      {
        xml.skipCurrentElement();
      }
    }

    return connections;
  }

  Component *ComponentGraphReader::constructComponent(const QString &/*type*/, const QString &name)
  {
    // TODO: actually check the type
    return new LuaComponent(name);
  }
}
