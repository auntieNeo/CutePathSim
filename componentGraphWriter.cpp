#include "componentGraphWriter.h"
#include "component.h"
#include "componentGraph.h"

namespace CutePathSim
{
  ComponentGraphWriter::ComponentGraphWriter(ComponentGraph *componentGraph)
  {
    m_componentGraph = componentGraph;
    xml.setAutoFormatting(true);
  }

  ComponentGraphWriter::~ComponentGraphWriter()
  {
  }

  bool ComponentGraphWriter::writeFile(QIODevice *device)
  {
    xml.setDevice(device);
    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE cutepathsim>");
    writeComponentGraph(m_componentGraph);
    return true;
  }

  void ComponentGraphWriter::writeComponentGraph(ComponentGraph *componentGraph)
  {
    xml.writeStartElement("component_graph");
    foreach(Component *component, componentGraph->components())
    {
      writeComponent(component);
    }
    xml.writeEndElement();  // component_graph
  }

  void ComponentGraphWriter::writeComponent(Component *component)
  {
    xml.writeStartElement("component");
    xml.writeAttribute("name", component->name());
    // write the inputs
    foreach(Component::Input *input, component->getInputs())
    {
      xml.writeEmptyElement("input");
      xml.writeAttribute("name", input->name());
    }
    // write the outputs
    foreach(Component::Output *output, component->getOutputs())
    {
      xml.writeStartElement("output");
      xml.writeAttribute("name", output->name());
      foreach(Component::Input *connectedInput, output->connections())
      {
        xml.writeEmptyElement("connection");
        if(connectedInput->internal())
        {
          xml.writeAttribute("internal", "true");
          xml.writeAttribute("input", connectedInput->name());
        }
        else
        {
          xml.writeAttribute("internal", "false");
          xml.writeAttribute("component", connectedInput->component()->name());
          xml.writeAttribute("input", connectedInput->name());
        }
      }
      xml.writeEndElement();  // output
    }
    // write the component sub-graph
    writeComponentGraph(component->subGraph());
    xml.writeEndElement();  // component
  }
}
