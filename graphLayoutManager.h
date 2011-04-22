#ifndef GRAPH_LAYOUT_MANAGER_H_
#define GRAPH_LAYOUT_MANAGER_H_

#include <QQueue>
#include <QEvent>
#include <QHash>

#include "component.h"

namespace CutePathSim
{
  class ComponentGraph;

  class GraphLayoutManager : public QObject
  {
    Q_OBJECT

    private:
      static GraphLayoutManager *m_instance;

      GraphLayoutManager();
//      explicit GraphLayoutManager(const GraphLayoutManager &);
      ~GraphLayoutManager();

      void processQueue();

      QQueue<ComponentGraph*> m_graphs;

      static QEvent::Type m_layoutGraphEventType;

    public:
      static GraphLayoutManager *instance();

      void scheduleLayoutGraph(ComponentGraph *graph);
      void remove(ComponentGraph *graph);

      bool event(QEvent *e);
  };
}

#endif
