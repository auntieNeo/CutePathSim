#include <cstdlib>

#include "32_bit_adder.h"

using namespace std;

namespace CutePathSim
{
  //Constructor: takes pointers to the two arrays that are being summed as inputs. Returns a pointer to a 32 bit array of
  //the sum of the two arrays
  ttBitAdder::ttBitAdder(bool *array1, bool *array2)
  {
    array[0].set(&array1[0], &array2[0], false);

    for(int i = 1; i < 32; i++)
    {
      array[i].set(&array1[i], &array2[i], array[i-1].Cout());
    }

    for(int i = 0; i < 32; i++)
    {
      sumArray[i] = array[i].S();
    }

    //Carry bit
    /*    bool carry = false;

          for(int count = 31, count >= 0, count --)
          {
          adder addA(array1[count], array2[count], carry);
          carry = adder.Cout();
          sumArray[count] = adder.S();
          }

          return retVal;
     */
  }

  ttBitAdder::~ttBitAdder()
  {
    delete [] sumArray;
  }
}
