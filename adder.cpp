#include <cstdlib>

using namespace std;

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

//Constructor: takes two bools that are being summed (a and b) and the carry-in bit (cin)
adder::adder(bool a, bool b, bool cin)
{
    set(a, b, cin);
}

adder::adder()
{

}

void adder::set(bool a, bool b, bool cin)
{
        //Assign variables
    A = a;
    B = b;
    Cin = cin;
    //Perform Boolean Logic
    D = (A ^= B);
    E = (D && Cin);
    F = (A && B);

//    S = !(D | Cin);
 //   Cout = (E | F);
}
//Returns Sum bit
bool adder::S()
{
    return (D ^= Cin);
}
//Returns Carry bit
bool adder::Cout()
{
    return (D && Cin) | (A && B));
}
