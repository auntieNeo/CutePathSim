#include <QMenuBar>
#include <QSignalMapper>

#include "componentGraph.h"
#include "componentGraphView.h"
#include "mainWindow.h"
#include "mux.h"

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

    // FIXME: remove this test code
    // add some components to the graph
    m_componentGraph->addComponent(new Mux("Mux_01"));
  }

  MainWindow::~MainWindow()
  {
    // FIXME: do these menus even need to be deleted?
    /*
    delete m_fileMenu;
    delete m_newSimulationMenu;
    delete m_openSimulationMenu;
    */
    delete m_componentGraphView;
    delete m_componentGraph;
  }

  void MainWindow::newSimulation()
  {
  }

  void MainWindow::openSimulation()
  {
  }
}
