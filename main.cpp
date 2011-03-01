#include <QtGui>

#include "mainWindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  CutePathSim::MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}

/**
 * \mainpage CutePathSim Documentation
 * \section intro_section Introduction
 * CutePathSim is a graphical, object oriented simulation of the data path in a digital system.
 *
 * \section basics_section Basics
 * Something something something.
 *
 * \section structure_section Structure
 * \dot
 * digraph system
 * {
 *   node [shape=record];
 *   gui [ label="Qt GUI" ]
 *   component [ label="Component Graph Interface" ]
 *   graphviz [ label=Graphviz ]
 *   cpu [ label="Reference CPU" ]
 *   gui->component
 *   gui->graphviz
 *   component->cpu
 * }
 * \enddot
 *
 * \dot
 * digraph system
 * {
 *   node [shape=record];
 *   gui [ label="Qt GUI" ]
 *   systemc [ label="SystemC Interface" ]
 *   graphviz [ label=Graphviz ]
 *   cpu [ label="Reference CPU" ]
 *   lua [ label="Lua Interface" ]
 *   gui->systemc
 *   gui->graphviz
 *   systemc->cpu
 *   systemc->lua
 *   lua->cpu
 * }
 * \enddot
 *
 * \dot
 * digraph flow
 * {
 *   start
 *   end
 *   is_thread [label="Layout thread\nrunning?", shape=diamond]
 *   start->is_thread
 *   is_thread->end [label="yes"]
 *   is_finished_thread [label="Thread recently\nfinished layout?", shape=diamond]
 *   is_thread->is_finished_thread [label="no"]
 *   is_queue [label="Graphs in\nqueue?", shape=diamond]
 *   is_finished_thread->is_queue [label="no"]
 *   apply_layout [label="Apply graph layout\nto GUI items", shape=record]
 *   is_finished_thread->apply_layout [label="yes"]
 *   resize_graph [label="Display graph and\nresize to parent", shape=record]
 *   apply_layout->is_component_queue
 *   is_component_queue [label = "Components in\nresize queue?", shape=diamond]
 *   is_component_queue->resize_graph [label="no"]
 *   resize_component [label = "Take component from\nqueue and resize\ncomponent", shape=record]
 *   is_component_queue->resize_component [label="yes"]
 *   resize_component->is_component_queue
 *   resize_graph->is_queue
 *   is_queue->end [label="no"]
 *   take_graph [label="Take graph from queue", shape=record]
 *   is_queue->take_graph [label="yes"]
 *   layout_graph_thread [label="Start layout graph in thread", shape=record]
 *   take_graph->layout_graph_thread
 *   layout_graph_thread->start_thread [style=dashed]
 *   layout_graph_thread->end
 *
 *   start_thread [label="start thread"]
 *   end_thread [label="end thread"]
 *   layout_graph [label="Layout graph with Graphviz", shape=record]
 *   start_thread->layout_graph
 *   layout_graph->end_thread
 *   end_thread->is_finished_thread [style=dashed]
 *
 *   start_change_graph [label="start change graph"]
 *   change_graph [label="Change a graph", shape=record]
 *   schedule_graph [label="Schedule a graph re-layout", shape=record]
 *   end_change_graph [label="end change graph"]
 *   start_change_graph->change_graph
 *   change_graph->schedule_graph
 *   schedule_graph->end_change_graph
 *   schedule_graph->is_queue [style=dashed]
 *
 *   start_change_component [label="start change component"]
 *   change_component [label="Change a Component", shape=record]
 *   schedule_component [label="Schedule a component resize", shape=record]
 *   schedule_graph2 [label="Schedule a graph re-layout", shape=record]
 *   end_change_component [label="end change component"]
 *   start_change_component->change_component
 *   change_component->schedule_component
 *   schedule_component->schedule_graph2
 *   schedule_graph2->end_change_component
 *   schedule_component->is_component_queue [style=dashed]
 *   schedule_graph2->is_queue [style=dashed]
 * }
 * \enddot
 */
