#include <QDebug>
#include <QMenuBar>
#include <QSignalMapper>

#include "componentGraphScene.h"
#include "componentGraphView.h"
#include "mainWindow.h"
#include "components/testComponent.h"
#include "components/intGeneratorComponent.h"
#include "components/boolGeneratorComponent.h"
#include "components/printIntComponent.h"
#include "components/rippleCarryAdder.h"

namespace CutePathSim
{
  MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
  {
    setupUi(this);

    // add the graph view
    m_componentGraphView = new ComponentGraphView(this);
    m_componentGraphScene = new ComponentGraphScene();
    m_componentGraphView->setScene(m_componentGraphScene);
    setCentralWidget(m_componentGraphView);

    // FIXME: remove this test code
    IntGeneratorComponent *outputs42 = new IntGeneratorComponent("Outputs_42", 42);
    IntGeneratorComponent *outputs5 = new IntGeneratorComponent("Outputs_5", 5);
    PrintIntComponent *printInt = new PrintIntComponent("Print_Int");
    RippleCarryAdder *adder = new RippleCarryAdder("RippleCarryAdder", 32);
    m_componentGraphScene->addComponent(outputs42);
    m_componentGraphScene->addComponent(outputs5);
    m_componentGraphScene->addComponent(printInt);
    m_componentGraphScene->addComponent(adder);

    outputs42->getOutput("output")->connect(adder->getInput("a"));
    outputs5->getOutput("output")->connect(adder->getInput("b"));
    adder->getOutput("sum")->connect(printInt->getInput("input"));
    adder->setLayout(Component::EXPANDED);

    m_componentGraphScene->layoutGraph();

    // call run() manually... the order in which these are run will be determined by a sorting algorithm in the future
    outputs42->run();
    outputs5->run();
    adder->run();
    printInt->run();


    // populate the menus
    m_viewMenu->addAction(m_componentGraphView->zoomInAction());
    m_viewMenu->addAction(m_componentGraphView->zoomOutAction());
    m_viewMenu->addAction(m_componentGraphView->fitViewAction());

    // connect the signals
  }

  MainWindow::~MainWindow()
  {
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
