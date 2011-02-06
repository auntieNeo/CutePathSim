#include <QMenuBar>
#include <QSignalMapper>

#include "componentGraph.h"
#include "componentGraphView.h"
#include "mainWindow.h"

#define TEST 0

namespace CutePathSim
{
  MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
  {
    m_fileMenu = menuBar()->addMenu(tr("&File"));

    // populate the menu bar
    m_newSimulationMenu = m_fileMenu->addMenu(tr("&New Simulation"));
    m_openSimulationMenu = m_fileMenu->addMenu(tr("&Open Simulation..."));
    connect(m_newSimulationMenu, SIGNAL(triggered()), this, SLOT(newSimulation()));
    connect(m_openSimulationMenu, SIGNAL(triggered()), this, SLOT(openSimulation()));

    // add the graph view
    m_componentGraphView = new ComponentGraphView(this);
    m_componentGraph = new ComponentGraph();
    m_componentGraphView->setScene(m_componentGraph);
    setCentralWidget(m_componentGraphView);
  }

  MainWindow::~MainWindow()
  {
  }

  void MainWindow::newSimulation()
  {
  }

  void MainWindow::openSimulation()
  {
  }
}
