#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <QGraphicsView>
#include <QMainWindow>

#include "ui_mainWindow.h"

namespace CutePathSim
{
  class ComponentGraphScene;
  class ComponentGraphView;

  class MainWindow : public QMainWindow, private Ui_MainWindow
  {
    Q_OBJECT

    public:
      MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
      ~MainWindow();

    private slots:
      void newSimulation();
      void openSimulation();

    private:
      ComponentGraphScene *m_componentGraphScene;
      ComponentGraphView *m_componentGraphView;
  };
}

#endif
