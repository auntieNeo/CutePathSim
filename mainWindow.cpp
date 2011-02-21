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

    /*
    // FIXME: remove this test code
    // construct all the components and add them to the graph scene
    TestComponent *test01 = new TestComponent("Test_01");
    IntGeneratorComponent *outputs42 = new IntGeneratorComponent("Outputs_42", 42);
    IntGeneratorComponent *outputs5 = new IntGeneratorComponent("Outputs_5", 5);
    BoolGeneratorComponent *outputsTrue = new BoolGeneratorComponent("Outputs_True", true);
    PrintIntComponent *printInt = new PrintIntComponent("Print_Int");
    m_componentGraphScene->addComponent(test01);
    m_componentGraphScene->addComponent(outputs42);
    m_componentGraphScene->addComponent(outputs5);
    m_componentGraphScene->addComponent(outputsTrue);
    m_componentGraphScene->addComponent(printInt);


    // manually feed data into the test component, to test it
    outputs42->getOutput("output")->connect(test01->getInput("input_01"));
    outputs5->getOutput("output")->connect(test01->getInput("input_02"));
    outputsTrue->getOutput("output")->connect(test01->getInput("multiplyFlag"));
    test01->getOutput("output")->connect(printInt->getInput("input"));
    */

    RippleCarryAdder *adder = new RippleCarryAdder("RippleCarryAdder", 8);
    m_componentGraphScene->addComponent(adder);

    m_componentGraphScene->layoutGraph();

    /*
    // call run() manually... the order in which these are run will be determined by a sorting algorithm in the future
    outputs42->run();
    outputs5->run();
    outputsTrue->run();
    test01->run();
    printInt->run();

    test01->setLayout(Component::EXPANDED);
    */
    adder->setLayout(Component::EXPANDED);

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
