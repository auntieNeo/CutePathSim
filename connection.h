#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <QHash>

namespace CutePathSim
{
  class Connection : public QObject
  {
    Q_OBJECT

    public:
      Connection(const QString &name, QGraphicsItem *parent = 0);
      ~Connection();

    private:
      QHash<Input *, QString>;
      QHash<Output *, QString>;
  };
}

#endif
