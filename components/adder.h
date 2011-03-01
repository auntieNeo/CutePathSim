#ifndef ADDER_H_
#define ADDER_H_

namespace CutePathSim
{
  class adder
  {
    private:
      //Declare Variables
      bool A, B, Cin;
      bool D, E, F;

    public:
      /* Constructor */
      adder(bool, bool, bool);
      adder();

      void set(bool, bool, bool);

      bool S();
      bool Cout();
  };
}

#endif
