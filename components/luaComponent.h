#ifndef LUA_COMPONENT_H_
#define LUA_COMPONENT_H_

#include <QString>

#include "component.h"

namespace CutePathSim
{
  class LuaComponent : public Component
  {
    public:
      LuaComponent(const QString &name, QGraphicsItem *parent = 0);
      ~LuaComponent();
  };
}

#endif
