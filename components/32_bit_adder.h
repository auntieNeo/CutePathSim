#ifndef TT_BIT_ADDER_H_
#define TT_BIT_ADDER_H_

#include "adder.h"

namespace CutePathSim
{
  //32 bit adder class
  class ttBitAdder
  {
    public:
      //Constructor declaration
      ttBitAdder(bool *array1, bool *array2);
      ~ttBitAdder();

    private:
      // Array where sum is stored
      bool *sumArray;
      adder array[32];
  };
}

#endif
