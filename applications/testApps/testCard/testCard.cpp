#include<fstream>
#include<string>
#include<Card.h>
#include<dsConfig.h>
#include<types.h>

using namespace ds;
int main()
{
    std::cout << "Constructing from deckIndex..." << std::endl;
    for (short i = 0; i < 52; ++i) {
        Card diCard(i);
        std::cout << i << ": " << diCard << std::endl;
    }
    std::string asStr("AS");
    std::cout << "Reading from string...";
    Card as(asStr);
    std::cout << as << std::endl;
    std::cout << "Reading from char array...";
    Card as2("AS");
    std::cout << as2 << std::endl;
    std::cout << "Reading from static const objects..." << std::endl;
    Card twoC(Card::two, Card::clubs);
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
        Card cr(is);
        std::cout << "Read '" << cr << "'" << std::endl;
    }
    return 0;
}
