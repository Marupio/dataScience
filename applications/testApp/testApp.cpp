#include <card.h>
#include <dsConfig.h>
#include <fstream>

using namespace ds;
int main()
{
    card as('A','S');
    card twoC(2, card::CLUBS);
    std::cout << as << twoC << std::endl;
    std::cout << "Reading from file..." << std::endl;
    std::ifstream is("inputFile");
    if (!is.is_open())
    {
        std::cerr << "Error! File cannot be openned." << std::endl;
        std::abort();
    }
    bool eofReached = false;
    while (1){
        if (std::isspace(is.peek())) {
            is.get();
            continue;
        }
        else if (is.peek() == EOF) {
            break;
        }
        card cr(is);
        std::cout << "Read '" << cr << "'" << std::endl;
    }
    return 0;
}
