#include "assembler.h"
#include <QList>
#include <QStringList>

const unsigned int ADD_OP = 0x00;
const unsigned int OPSHIFT = 26;
const unsigned int FuncShift = 0;
const unsigned int SUB_OP = 0x00;
const unsigned int ADD_function = 0x20;
const unsigned int SUB_function = 0x22;
const unsigned int Rs = 0x01;
const unsigned int Rt = 0x02;
const unsigned int Rd = 0x03;
const unsigned int RSshift_ADDSUB = 21;
const unsigned int RTshift_ADDSUB = 16;

QByteArray parseAssembly(QTextStream assembly)
{

    while (!assembly.atEnd())
    {
        unsigned int instruction = 0;

        QString line1 = assembly.readLine();
        QStringList words = line1.split(' ');

        if (words[0] == "ADD")
        {
            instruction |= ADD_OP << OPSHIFT;
            instruction |= ADD_function << FuncShift;

        }

        if (words[0] == "SUB")
        {
            instruction |= SUB_OP << OPSHIFT;
            instruction |= SUB_function << FuncShift;
        }

        QStringList Registers = words[1].split(",");

        if (Registers[0] == "$1")
        {
            instruction |= Rs << RSshift_ADDSUB;
        }

        if (Registers[0] == "$2")
        {
            instruction |= Rs << RSshift_ADDSUB;
        }

        if (Registers[0] == "$3")
        {
            instruction |= Rs << RSshift_ADDSUB;
        }

        if (Registers[1] == "$1")
        {
            instruction |= Rt << RTshift_ADDSUB;
        }

        if (Registers[1] == "$2")
        {
            instruction |= Rt << RTshift_ADDSUB;
        }

        if (Registers[1] == "$3")
        {
            instruction |= Rt << RTshift_ADDSUB;
        }

    }
}
