#include <card.h>
#include <dsConfig.h>
#include <fstream>
#include <string>

using namespace ds;
int main()
{
    std::cout << "Constructing from deckIndex..." << std::endl;
    for (short i = 0; i < 52; ++i) {
        card diCard(i);
        std::cout << i << ": " << diCard << std::endl;
    }
    std::string asStr("AS");
    std::cout << "Reading from string...";
    card as(asStr);
    std::cout << as << std::endl;
    std::cout << "Reading from char array...";
    card as2("AS");
    std::cout << as2 << std::endl;
    std::cout << "Reading from static const objects..." << std::endl;
    card twoC(card::two, card::clubs);
    std::cout << twoC << std::endl;
    std::cout << "Reading from file..." << std::endl;
    std::ifstream is("inputFile");
    if (!is.is_open())
    {
        std::cerr << "Error! File cannot be opened." << std::endl;
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
