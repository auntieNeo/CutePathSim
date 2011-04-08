#include <QDebug>
#include <QMenuBar>
#include <QSignalMapper>

#include "common.h"
#include "componentGraphScene.h"
#include "componentGraphView.h"
#include "mainWindow.h"
#include "components/testComponent.h"
#include "components/intGeneratorComponent.h"
#include "components/boolGeneratorComponent.h"
#include "components/printIntComponent.h"
#include "components/rippleCarryAdder.h"
#include "components/binaryMultiplier.h"

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
    BinaryMultiplier *multiplier = new BinaryMultiplier("BinaryMultiplier", 8);
    m_componentGraphScene->addComponent(outputs42);
    m_componentGraphScene->addComponent(outputs5);
    m_componentGraphScene->addComponent(printInt);
    m_componentGraphScene->addComponent(multiplier);

    outputs42->getOutput("output")->connect(multiplier->getInput("a"));
    outputs5->getOutput("output")->connect(multiplier->getInput("b"));
    multiplier->getOutput("product")->connect(printInt->getInput("input"));
    multiplier->setLayout(Component::EXPANDED);

    // call run() manually... the order in which these are run will be determined by a sorting algorithm in the future
    outputs42->run();
    outputs5->run();
    multiplier->run();
    printInt->run();

    // populate the menus
    m_viewMenu->addAction(m_componentGraphView->zoomInAction());
    m_viewMenu->addAction(m_componentGraphView->zoomOutAction());
    m_viewMenu->addAction(m_componentGraphView->fitViewAction());
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

  void MainWindow::runSimulation()
  {
    /*
    // use a topological sort to determine execution order
    QList<Component *> sortedNodes;
    foreach(Component *component, m_componentGraphScene->rootGraph()
    */
  }

  void MainWindow::addDock(QWidget *widget)
  {
    if(m_guiDocks.contains(widget))
      return;

    QDockWidget *dock = new QDockWidget(widget->windowTitle(), this);
    dock->setWidget(widget);

    addDockWidget(Qt::LeftDockWidgetArea, dock);
  }

  void MainWindow::removeDock(QWidget *widget)
  {
    if(!m_guiDocks.contains(widget))
      return;

    delete m_guiDocks.take(widget);
    return;
  }

  bool MainWindow::event(QEvent *event)
  {
    if(event->type() == ComponentDockEvent::EventType)
    {
      addDock(static_cast<ComponentDockEvent*>(event)->component()->getToolBox());
      return true;
    }

    return QObject::event(event);
  }

  MainWindow *mainWindow;
}
