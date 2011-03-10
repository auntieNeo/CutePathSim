#ifndef GRAPH_LAYOUT_MANAGER_H_
#define GRAPH_LAYOUT_MANAGER_H_

namespace CutePathSim
{
  class GraphLayoutManager
  {
    private:
      class GraphLayoutThread : public QThread
      {
        public:
          GraphLayoutThread();
          ~GraphLayoutThread();

          void run();

          bool recentlyFinished() { return m_recentlyFinished; }

          ComponentGraph *graph() { return m_graph; }
          void setGraph(ComponentGraph *graph) { m_graph = graph;}

        private:
          ComponentGraph *m_graph;
          bool m_recentlyFinished;
      };
      GraphLayoutManager();
      ~GraphLayoutManager();

      void scheduleLayoutGraph(ComponentGraph *graph);
      void processQueue();

    public:
      GraphLayoutManager *GraphLayoutManager *instance();

      QQueue<ComponentGraph*> m_graphs;
      GraphLayoutThread m_thread;
  };
}

#endif
