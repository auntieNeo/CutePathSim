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

      struct Input
      {
        public:
          // function pointer here :P
          QString name;
      };

      struct Output
      {
        public:
          // function pointer here :P
          QString name;
      }

    private:
      QHash</*function pointer here*/, QString> inputs;
      QHash</*function pointer here*/, QString> outputs;
      QHash<
  };
}

#endif
