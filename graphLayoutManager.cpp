#include "componentGraph.h"
#include "graphLayoutManager.h"

namespace CutePathSim
{
  /**
   * \class GraphLayoutManager
   * The GraphLayoutManager class is responsible for managing when the component graphs get layed out and the thread it is layed out in. The scheduleLayoutGraph() method can be used to schedule a graph layout, which will layout the graph in a thread using Graphviz. Because the GraphLayoutManager is a singleton and manages all of the graphs, scheduleLayoutGraph() can be called multiple times for the same graph without causing that graph to be layed out more than once.
   */
  GraphLayoutManager::GraphLayoutManager()
  {
    m_thread = new GraphLayoutThread();
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
      m_thread->graph()->updateItemPositions();
      // TODO: resize the components that need to be resized
    }

    if(!m_graphs.isEmpty())
    {
      ComponentGraph *graph = m_graphs.dequeue();
      graph->updateNodeSizes();  // update the sizes of the Graphviz nodes to reflect the anticipated size of the components we will resize afterwards
      m_thread->setGraph(graph);
      // TODO: add the components to be resized to the thread's data members, so we know what to resize after the thread is done
      m_thread->run();
    }
  }

  GraphLayoutManager::GraphLayoutThread::GraphLayoutThread()
  {
    m_recentlyFinished = false;
  }

  GraphLayoutManager::GraphLayoutThread::~GraphLayoutThread()
  {
  }

  void GraphLayoutManager::GraphLayoutThread::run()
  {
    m_recentlyFinished = false;

    m_graph->layoutGraph();
  }
}
