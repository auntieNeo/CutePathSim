#include "graphLayoutManager.h"

namespace CutePathSim
{
  GraphLayoutManager::GraphLayoutManager()
  {
    m_thread = new GraphLayoutThread();
  }

  GraphLayoutManager::~GraphLayoutManager()
  {
  }

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
    if(!m_graphs.contains(graph))
    {
      m_graphs.enqueue(graph);
    }
  }

  void GraphLayoutManager::processQueue()
  {
    if(m_thread->isRunning())
    {
      return;
    }

    if(m_thread->recentlyFinished())
    {
      // apply the recently generated layout to the GUI items
      m_graph->updateItemPositions();
      // TODO: resize the components that need to be resized
    }

    if(!m_graphs.isEmpty())
    {
      m_graph->updateNodeSizes();  // update the sizes of the Graphviz nodes to reflect the anticipated size of the components we will resize afterwards
      m_thread->setGraph(m_graphs.dequeue());
      // TODO: add the components to be resized to the thread's data members, so we know what to resize after the thread is done
      m_thread->run();
    }
  }

  GraphLayoutManager::GraphLayoutThread()
  {
    m_recentlyFinished = false;
  }

  GraphLayoutManager::~GraphLayoutThread()
  {
  }

  void GraphLayoutManager::GraphLayoutThread::run()
  {
    m_recentlyFinished = false;

    m_graph->layoutGraph();
  }
}
