#include <QMenuBar>
#include <QSignalMapper>

#include "board.h"
#include "boardView.h"
#include "mainWindow.h"

#define TEST 0

namespace CutePathSim
{
  MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
  {
    m_fileMenu = menuBar()->addMenu(tr("&File"));

    // populate the new game menu
    m_newSimulationMenu = m_fileMenu->addMenu(tr("&New Simulation"));
    m_openSimulationMenu = m_fileMenu->addMenu(tr("&Open Simulation..."));
    connect(m_newSimulationMenu, SIGNAL(triggered()), this, SLOT(newSimulation()));
    connect(m_openSimulationMenu, SIGNAL(triggered()), this, SLOT(openSimulation()));

    // add the board view
    m_boardView = new BoardView(this);
    m_board = new Board();
    m_boardView->setScene(m_board);
    setCentralWidget(m_boardView);
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
