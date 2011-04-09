#include "assembler.h"
#include <QList>
#include <QStringList>
#include <QVariant>
#include <QtEndian>

const unsigned int ADD_OP = 0x00;
const unsigned int SUB_OP = 0x00;

const unsigned int OP_SHIFT = 26;
const unsigned int FUNC_SHIFT = 0;
const unsigned int RS_SHIFT = 21;
const unsigned int RT_SHIFT = 16;
const unsigned int RD_SHIFT = 11;

const unsigned int OP_MASK = 0x3F << OP_SHIFT;

const unsigned int ADD_FUNC = 0x20;
const unsigned int SUB_FUNC = 0x22;
const unsigned int RSshift_ADDSUB = 21;
const unsigned int RTshift_ADDSUB = 16;

const unsigned int NUM_REGISTERS = 32;

namespace CutePathSim
{
  QByteArray parseAssembly(QTextStream &assembly)
  {
    QByteArray result;

    while (!assembly.atEnd())
    {
      unsigned int instruction = 0;

      QString line1 = assembly.readLine();
      QStringList words = line1.split(' ');

      // parse the instruction name
      if (words[0] == "ADD")
      {
        instruction |= ADD_OP << OP_SHIFT;
        instruction |= ADD_FUNC << FUNC_SHIFT;

      }
      else if (words[0] == "SUB")
      {
        instruction |= SUB_OP << OP_SHIFT;
        instruction |= SUB_FUNC << FUNC_SHIFT;
      }
      else
      {
        Q_ASSERT(false);  // TODO: throw an exception
      }


      switch((instruction & OP_MASK) >> OP_SHIFT)
      {
        case ADD_OP:
          //      case SUB_OP:
          {
            // parse the register numbers
            QRegExp rx("^\\s.\\$(\\d+)\\s.,\\s.\\$(\\d+)\\s.,\\s.\\$(\\d+)\\s.$");
            int pos = rx.indexIn(words[1]);
            if(pos < 0)
            {
              Q_ASSERT(false);  // TODO: throw an exception
            }
            QStringList registers = rx.capturedTexts();
            if(registers.length() != 3)
            {
              Q_ASSERT(false);  // TODO: throw an exception
            }
            unsigned int rd = QVariant(registers[0]).toInt();
            if(rd >= NUM_REGISTERS)
              Q_ASSERT(false);  // TODO: throw an exception
            unsigned int rs = QVariant(registers[1]).toInt();
            if(rs >= NUM_REGISTERS)
              Q_ASSERT(false);  // TODO: throw an exception
            unsigned int rt = QVariant(registers[2]).toInt();
            if(rt >= NUM_REGISTERS)
              Q_ASSERT(false);  // TODO: throw an exception

            instruction |= rd << RD_SHIFT;
            instruction |= rs << RS_SHIFT;
            instruction |= rt << RT_SHIFT;
          }
        default:
          Q_ASSERT(false);  // TODO: throw an exception
      }
      result.append(qToLittleEndian(instruction));
    }

    return result;
  }
}
