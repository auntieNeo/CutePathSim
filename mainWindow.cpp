#include <QDebug>
#include <QMenuBar>
#include <QSignalMapper>

#include "componentGraphScene.h"
#include "componentGraphView.h"
#include "mainWindow.h"
//#include "mux.h"
#include "testComponent.h"
#include "intGeneratorComponent.h"
#include "boolGeneratorComponent.h"

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
    m_componentGraphScene = new ComponentGraphScene();
    m_componentGraphView->setScene(m_componentGraphScene);
    setCentralWidget(m_componentGraphView);

    // FIXME: remove this test code
    /*
    // add some components to the graph
    Mux *mux01, *mux02, *mux03;
    mux01 = new Mux("Mux_01");
    mux02 = new Mux("Mux_02");
    mux03 = new Mux("Mux_03");
    m_componentGraphScene->addComponent(mux01);
    m_componentGraphScene->addComponent(mux02);
    m_componentGraphScene->addComponent(mux03);
    mux01->getOutput("output")->connect(mux01->getInput("firstInput"));
    mux01->getOutput("output")->connect(mux03->getInput("secondInput"));
    mux01->getOutput("output")->connect(mux02->getInput("secondInput"));
    mux02->getOutput("output")->connect(mux03->getInput("firstInput"));
    mux03->getOutput("output")->connect(mux02->getInput("firstInput"));
    */

    TestComponent *test01 = new TestComponent("Test_01");
    IntGeneratorComponent *outputs42 = new IntGeneratorComponent("Outputs_42", 42);
    IntGeneratorComponent *outputs5 = new IntGeneratorComponent("Outputs_5", 5);
    BoolGeneratorComponent *outputsTrue = new BoolGeneratorComponent("Outputs_True", true);
    m_componentGraphScene->addComponent(test01);
    m_componentGraphScene->addComponent(outputs42);
    m_componentGraphScene->addComponent(outputs5);
    m_componentGraphScene->addComponent(outputsTrue);


    // manually feed data into the test component, to test it
    outputs42->getOutput("output")->connect(test01->getInput("input_01"));
    outputs5->getOutput("output")->connect(test01->getInput("input_02"));
    outputsTrue->getOutput("output")->connect(test01->getInput("multiplyFlag"));

    m_componentGraphScene->layoutGraph();

    // call run() manually... the order in which these are run will be determined by a sorting algorithm in the future
    outputs42->run();
    outputs5->run();
    outputsTrue->run();
    test01->run();
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
    delete m_componentGraphScene;
  }

  void MainWindow::newSimulation()
  {
  }

  void MainWindow::openSimulation()
  {
  }
}
