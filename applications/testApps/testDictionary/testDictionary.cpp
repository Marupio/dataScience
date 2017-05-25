#include <climits>
#include <fstream>
#include <string>
#include <sstream>
#include <Dictionary.h>
#include <dsConfig.h>

using namespace ds;
int main()
{
    Dictionary dict("testDict");
    bool quit = false;
    while(!quit) {
        std::cout << "Current dictionary = [\n" << dict << "\n]" << std::endl;
        std::cout << "\nSelect action:\n1 - found\n2 - foundType\n3 - lookup\n4 - isTokens"
            << "\n5 - lookupTokens\n6 - isDict\n7 - lookupDict\n"
            << "0 - quit\n"
            << "What do you want to do? ";
        int action;
        std::cin >> action;
        switch (action) {
            case 1: {
                std::cout << "found(key): What is key? ";
                std::string key;
                std::cin >> key;
                std::cout << "Result is " << dict.found(key) << std::endl;
                break;
            }
            case 2: {
                std::cout << "foundType(key): What is key? ";
                std::string key;
                std::cin >> key;
                std::cout << "Result is " << Entry::typeEnumToString(dict.foundType(key))
                    << std::endl;
                break;
            }
            case 3: {
                std::cout << "lookup(key): What is key? ";
                std::string key;
                std::cin >> key;
                const Entry& e(dict.lookup(key));
                std::cout << "Result is [";
                e.debugWrite(std::cout);
                std::cout << "]" << std::endl;
                break;
            }
            case 4: {
                std::cout << "isTokens(key): What is key? ";
                std::string key;
                std::cin >> key;
                std::cout << "Result is " << dict.isTokens(key) << std::endl;
                break;
            }
            case 5: {
                std::cout << "lookupTokens(key): What is key? ";
                std::string key;
                std::cin >> key;
                const VecToken& vt(dict.lookupTokens(key));
                std::cout << "Result is [";
                vt.debugWrite(std::cout);
                std::cout << "]" << std::endl;
                break;
            }
            case 6: {
                std::cout << "isDict(key): What is key? ";
                std::string key;
                std::cin >> key;
                std::cout << "Result is " << dict.isDict(key) << std::endl;
                break;
            }
            case 7: {
                std::cout << "lookupDict(key): What is key? ";
                std::string key;
                std::cin >> key;
                const Dictionary& subDict(dict.lookupDict(key));
                std::cout << "Result is [" << subDict << "]" << std::endl;
                break;
            }
            case 0: {
                quit = true;
                break;
            }
            default: {
                std::cout << "Unrecognized command" << std::endl;

                break;
            }
        }
    }
    std::ofstream os("output");
    dict.debugWrite(os);
    os << "-------------" << std::endl;
    os << dict << std::endl;
//    std::cout << dict << std::endl;
    return 0;
}
