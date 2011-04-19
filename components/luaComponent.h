#ifndef LUA_COMPONENT_H_
#define LUA_COMPONENT_H_

#include <QString>

#include "component.h"
#include "ui_interfacesWidget.h"

namespace CutePathSim
{
  class InterfacesWidget : public QWidget, private Ui_InterfacesWidget
  {
    Q_OBJECT

    public:
      InterfacesWidget(Component *component, QWidget *parent = 0);
      virtual ~InterfacesWidget();

    private:
      Component *m_component;

    private slots:
      void addInput();
      void removeInput();
      void addOutput();
      void removeOutput();
  };

  class LuaComponent : public Component
  {
    public:
      LuaComponent(const QString &name, QGraphicsItem *parent = 0);
      virtual ~LuaComponent();

      void run();

      QColor color() { return QColor(0xD6, 0xD6, 0xA6); }  // light brown

    protected:
      QToolBox *getToolBox();
      void closeToolBox();

    private:
      InterfacesWidget *m_interfacesWidget;
  };
}

#endif
