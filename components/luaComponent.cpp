#include "luaComponent.h"
#include "inputDialogs.h"

#include <QMessageBox>

namespace CutePathSim
{
  LuaComponent::LuaComponent(const QString &name, QGraphicsItem *parent) : Component(name, parent)
  {
    m_interfacesWidget = 0;
  }

  LuaComponent::~LuaComponent()
  {
    // FIXME: delete all the inputs, probably in Component
  }

  void LuaComponent::run()
  {
  }

  QToolBox *LuaComponent::getToolBox()
  {
    QToolBox *toolBox = Component::getToolBox();

    if(m_interfacesWidget == 0)
    {
      m_interfacesWidget = new InterfacesWidget(this, toolBox);
      toolBox->addItem(m_interfacesWidget, QObject::tr("Interfaces"));
    }

    return toolBox;
  }

  void LuaComponent::closeToolBox()
  {
    if(m_interfacesWidget != 0)
    {
      delete m_interfacesWidget;
      m_interfacesWidget = 0;
    }
    Component::closeToolBox();
  }

  InterfacesWidget::InterfacesWidget(Component *component, QWidget *parent) : QWidget(parent)
  {
    setupUi(this);
    m_component = component;

    connect(m_addInput, SIGNAL(clicked()), this, SLOT(addInput()));
    connect(m_removeInput, SIGNAL(clicked()), this, SLOT(removeInput()));
    connect(m_addOutput, SIGNAL(clicked()), this, SLOT(addOutput()));
    connect(m_removeOutput, SIGNAL(clicked()), this, SLOT(removeOutput()));
  }

  InterfacesWidget::~InterfacesWidget()
  {
    return;
  }

  void InterfacesWidget::addInput()
  {
    QString name = promptString(tr("Enter a unique name for the input:"));
    if(name.isNull())
      return;
    if(m_component->getInput(name) != 0)
    {
      QMessageBox::warning(this, tr("Input name not unique"), tr("Input with that name already exists. Please enter a unique name."));
      return;
    }
    int width = promptNatural(tr("Enter the bus width for the input:"));
    if(width == -1)
      return;
    m_component->addInput(new Component::Input(name, width, m_component));
    int row = m_inputTable->rowCount();
    m_inputTable->insertRow(row);
    m_inputTable->setItem(row, 0, new QTableWidgetItem(name));
    m_inputTable->setItem(row, 1, new QTableWidgetItem(QVariant(width).toString()));
    m_inputTable->sortByColumn(0);
    m_inputTable->resizeColumnsToContents();
  }

  void InterfacesWidget::removeInput()
  {
    if(m_inputTable->currentRow() == -1)
      return;
    delete m_component->removeInput(m_inputTable->item(m_inputTable->currentRow(), 0)->text());
    m_inputTable->removeRow(m_inputTable->currentRow());
  }

  void InterfacesWidget::addOutput()
  {
    QString name = promptString(tr("Enter a unique name for the output:"));
    if(name.isNull())
      return;
    if(m_component->getOutput(name) != 0)
    {
      QMessageBox::warning(this, tr("Output name not unique"), tr("Input with that name already exists. Please enter a unique name."));
      return;
    }
    int width = promptNatural(tr("Enter the bus width for the output:"));
    if(width == -1)
      return;
    m_component->addOutput(new Component::Output(name, width, m_component));
    int row = m_outputTable->rowCount();
    m_outputTable->insertRow(row);
    m_outputTable->setItem(row, 0, new QTableWidgetItem(name));
    m_outputTable->setItem(row, 1, new QTableWidgetItem(QVariant(width).toString()));
    m_outputTable->sortByColumn(0);
    m_outputTable->resizeColumnsToContents();
  }

  void InterfacesWidget::removeOutput()
  {
    if(m_outputTable->currentRow() == -1)
      return;
    delete m_component->removeOutput(m_outputTable->item(m_outputTable->currentRow(), 0)->text());
    m_outputTable->removeRow(m_outputTable->currentRow());
  }
}
