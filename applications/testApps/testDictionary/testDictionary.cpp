#include <fstream>
#include <string>
#include <Dictionary.h>
#include <dsConfig.h>

using namespace ds;
int main()
{
    Dictionary dict("testDict");
    std::cout << dict << std::endl;
    return 0;
}
