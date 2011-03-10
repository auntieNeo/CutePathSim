#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <QTextStream>

namespace CutePathSim
{
  QByteArray parseAssembly(QTextStream assembly);
}

#endif // ASSEMBLER_H

