#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <QGraphicsView>
#include <QMainWindow>
#include <QDockWidget>
#include <QHash>

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

    public slots:
      void addDock(QWidget *widget);
      void removeDock(QWidget *widget);

    private slots:
      void newSimulation();
      void openSimulation();
      void runSimulation();

    private:
      ComponentGraphScene *m_componentGraphScene;
      ComponentGraphView *m_componentGraphView;
      QHash<QWidget *, QDockWidget *> m_guiDocks;
  };
}

#endif
