#include "componentGraph.h"
#include "graphLayoutManager.h"

#include <QCoreApplication>

namespace CutePathSim
{
  /**
   * \class GraphLayoutManager
   * The GraphLayoutManager class is responsible for managing when the component graphs get layed out and the thread it is layed out in. The scheduleLayoutGraph() method can be used to schedule a graph layout, which will layout the graph in a thread using Graphviz. Because the GraphLayoutManager is a singleton and manages all of the graphs, scheduleLayoutGraph() can be called multiple times for the same graph without causing that graph to be layed out more than once.
   */
  GraphLayoutManager::GraphLayoutManager()
  {
    if(m_layoutGraphEventType == -1)
    {
      m_layoutGraphEventType = static_cast<QEvent::Type>(QEvent::registerEventType());
    }
  }

  /*
  GraphLayoutManager::GraphLayoutManager(const GraphLayoutManager &)
  {
  }
  */

  GraphLayoutManager::~GraphLayoutManager()
  {
  }

  /**
   * Returns an instance of the graph layout manager. Because the graph layout manager is a singleton, this should be the only instance available.
   */
  GraphLayoutManager *GraphLayoutManager::instance()
  {
    if(m_instance == 0)
    {
      m_instance = new GraphLayoutManager();  // FIXME: delete this somewhere
    }
    return m_instance;
  }

  /**
   * Schedules \a graph to be layed out with Graphviz when the graph layout manager gets around to it.
   *
   * Graphviz will be run from a thread, so this operation will not adversely affect the main GUI thread.
   *
   * If this method is called numerous times on the same graph, the graph will only be layed out once, assuming the graph isn't already being layed out in a thread.
   */
  void GraphLayoutManager::scheduleLayoutGraph(ComponentGraph *graph)
  {
    if(m_graphs.size() == 0)
    {
      QCoreApplication::postEvent(this, new QEvent(m_layoutGraphEventType));
    }
    if(!m_graphs.contains(graph))
    {
      m_graphs.enqueue(graph);
    }
  }

  void GraphLayoutManager::processQueue()
  {
    while(!m_graphs.isEmpty())
    {
      ComponentGraph *graph = m_graphs.dequeue();
      graph->layoutGraph();
    }
  }

  bool GraphLayoutManager::event(QEvent *e)
  {
    if(e->type() == m_layoutGraphEventType)
    {
      e->accept();
      this->processQueue();
      return true;
    }
    return QObject::event(e);
  }

  GraphLayoutManager *GraphLayoutManager::m_instance = 0;
  QEvent::Type GraphLayoutManager::m_layoutGraphEventType = static_cast<QEvent::Type>(-1);
}
