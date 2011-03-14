#ifndef GRAPH_LAYOUT_MANAGER_H_
#define GRAPH_LAYOUT_MANAGER_H_

#include <QQueue>
#include <QThread>

namespace CutePathSim
{
  class ComponentGraph;

  class GraphLayoutManager : public QObject
  {
    Q_OBJECT

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

      GraphLayoutManager *m_instance;

      GraphLayoutManager();
//      explicit GraphLayoutManager(const GraphLayoutManager &);
      ~GraphLayoutManager();

      void scheduleLayoutGraph(ComponentGraph *graph);
      void processQueue();

    public:
      GraphLayoutManager *instance();

      QQueue<ComponentGraph*> m_graphs;
      GraphLayoutThread *m_thread;
  };
}

#endif
