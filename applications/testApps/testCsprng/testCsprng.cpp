#include <Csprng.h>

using namespace ds;

int main(int argc, char const *argv[])
{
    EntropyInterface ent;
    Csprng rnd(ent, "testCsprng");
    rnd.writeRandBytes(10);
    short maxShort = 32000;
    short randShort = rnd(maxShort);
    std::cout << "randShort = " << randShort << std::endl;
    int randInt = rnd(32);
    std::cout << "randInt = " << randInt << std::endl;
    long randLong = rnd((long)2000000000);
    std::cout << "randLong = " << randLong << std::endl;

    unsigned short randUShort = rnd((unsigned short)32000);
    std::cout << "randUShort = " << randUShort << std::endl;
    unsigned int randUInt = rnd((unsigned int)32);
    std::cout << "randUInt = " << randUInt << std::endl;
    unsigned long randULong = rnd((unsigned long)2000000000);
    std::cout << "randULong = " << randULong << std::endl;


    unsigned long long randULongLong = rnd((unsigned long long)500000000000);
    std::cout << "randULongLong = " << randULongLong << std::endl;
    return 0;
}