#ifndef COMPONENT_DATA_TABLE_H_
#define COMPONENT_DATA_TABLE_H_

#include <QAbstractItemModel>
#include <QTableView>
#include <QWidget>

namespace CutePathSim
{
  class Component;

  class ComponentDataModel : public QAbstractItemModel
  {
    Q_OBJECT

    public:
      ComponentDataModel(Component *component, QObject *parent = 0);
      ~ComponentDataModel();

      int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
      QVariant  data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
      QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
      QModelIndex parent ( const QModelIndex & index ) const;
      int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

    private:
      Component *m_component;
  };
}

#endif
