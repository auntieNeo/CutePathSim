#include "component.h"
#include "componentDataModel.h"

namespace CutePathSim
{
  ComponentDataModel::ComponentDataModel(Component *component, QObject *parent) : QAbstractItemModel(parent)
  {
    m_component = component;
  }

  ComponentDataModel::~ComponentDataModel()
  {
  }

  int ComponentDataModel::columnCount(const QModelIndex & parent) const
  {
    return 0;
  }

  QVariant ComponentDataModel::data(const QModelIndex & index, int role) const
  {
    return QVariant();
  }

  QModelIndex ComponentDataModel::index(int row, int column, const QModelIndex & parent) const
  {
    return QModelIndex();
  }

  QModelIndex ComponentDataModel::parent(const QModelIndex & index) const
  {
    return QModelIndex();
  }

  int ComponentDataModel::rowCount(const QModelIndex & parent) const
  {
    return m_component->m_data.size();
  }
}
