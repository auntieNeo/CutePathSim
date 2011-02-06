#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <QGraphicsView>
#include <QMainWindow>

namespace CutePathSim
{
  class ComponentGraph;
  class ComponentGraphView;

  class MainWindow : public QMainWindow
  {
    Q_OBJECT

    public:
      MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
      ~MainWindow();

    private slots:
      void newSimulation();
      void openSimulation();

    private:
      QMenu *m_fileMenu, *m_newSimulationMenu, *m_openSimulationMenu;
      ComponentGraph *m_componentGraph;
      ComponentGraphView *m_componentGraphView;
  };
}

#endif
