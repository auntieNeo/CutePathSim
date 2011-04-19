#include <QDebug>
#include <QMenuBar>
#include <QSignalMapper>
#include <QMessageBox>

#include "common.h"
#include "componentGraph.h"
#include "componentGraphScene.h"
#include "componentGraphView.h"
#include "mainWindow.h"
#include "inputDialogs.h"
#include "components/testComponent.h"
#include "components/intGeneratorComponent.h"
#include "components/boolGeneratorComponent.h"
#include "components/printIntComponent.h"
#include "components/rippleCarryAdder.h"
#include "components/binaryMultiplier.h"
#include "components/instructionFetcher.h"
#include "components/arithmeticLogicUnit.h"
#include "components/controlUnit.h"
#include "components/registers.h"
#include "components/luaComponent.h"

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
    IntGeneratorComponent *outputs42 = new IntGeneratorComponent("Outputs_14", 14);
    IntGeneratorComponent *outputs5 = new IntGeneratorComponent("Outputs_3", 3);
    PrintIntComponent *printInt = new PrintIntComponent("Print_Int");
    BinaryMultiplier *multiplier = new BinaryMultiplier("BinaryMultiplier", 8);
    multiplier->setLayout(Component::EXPANDED);
    m_componentGraphScene->addComponent(outputs42);
    m_componentGraphScene->addComponent(outputs5);
    m_componentGraphScene->addComponent(printInt);
    m_componentGraphScene->addComponent(multiplier);

    InstructionFetcher *instructionFetcher = new InstructionFetcher("InstructionFetcher");
    m_componentGraphScene->addComponent(instructionFetcher);

    outputs42->getOutput("output")->connect(multiplier->getInput("a"));
    outputs5->getOutput("output")->connect(multiplier->getInput("b"));
    multiplier->getOutput("product")->connect(printInt->getInput("input"));
    */

    ControlUnit *controlUnit = new ControlUnit("Control Unit");
    m_componentGraphScene->addComponent(controlUnit);

    InstructionFetcher *instructionFetcher = new InstructionFetcher("Instruction Fetcher");
    m_componentGraphScene->addComponent(instructionFetcher);

    ArithmeticLogicUnit *alu = new ArithmeticLogicUnit("ALU");
    m_componentGraphScene->addComponent(alu);

    Registers *registers = new Registers("Registers");
    m_componentGraphScene->addComponent(registers);

    LuaComponent *lua = new LuaComponent("Lua Component");
    m_componentGraphScene->addComponent(lua);

    instructionFetcher->getOutput("output")->connect(controlUnit->getInput("instruction"));

    m_componentGraphScene->rootGraph()->run();

    // populate the menus
    m_viewMenu->addAction(m_componentGraphView->zoomInAction());
    m_viewMenu->addAction(m_componentGraphView->zoomOutAction());
    m_viewMenu->addAction(m_componentGraphView->fitViewAction());

    // connect the signals/slots
    connect(m_addComponent, SIGNAL(activated()), this, SLOT(addComponent()));
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
  }

  void MainWindow::addComponent()
  {
    QString name = promptString(tr("Enter a unique name for the component."));
    if(m_componentGraphScene->rootGraph()->getComponent(name) != 0)
    {
      QMessageBox::warning(this, tr("Component name not unique"), tr("Component with that name already exists. Please enter a unique name."));
      return;
    }
    m_componentGraphScene->rootGraph()->addComponent(new LuaComponent(name));
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

    return QMainWindow::event(event);
  }

  MainWindow *mainWindow;
}
