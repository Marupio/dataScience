#include <fstream>
#include <string>
#include <PktCards.h>
#include <dsConfig.h>
#include <types.h>

using namespace ds;
int main()
{
    std::cout << "Constructing from deckIndex..." << std::endl;
    for (short i = 0; i < 20; ++i) {
        DeckInd diA(std::rand()%52);
        DeckInd diB(std::rand()%52);
        PktCards pc(diA, diB);
        std::cout << pc << " ";
    }
    std::cout << std::endl;

    std::cout << "Testing comparison:\n";
    DeckInd diA = 35;
    DeckInd diB = 27;
    PktCards pcA(diA, diB);
    PktCards pcB(diB, diA);
    if (pcA == pcB) {
        std::cout << "success" << std::endl;
    } else {
        std::cout << "fail" << std::endl;
    }

    PktCards pcD
    (
        Card(Card::wildValue, Card::clubs),
        Card(Card::two, Card::wildSuit)
    );

    PktCards pcE
    (
        Card::seven, Card::clubs,
        Card::two, Card::hearts
    );

    if (pcD == pcE) {
        std::cout << pcD << " == " << pcE << std::endl;
    } else {
        std::cout << pcD << " != " << pcE << std::endl;
    }
    
    std::ifstream is("inputFilePkt");
    if (!is.is_open())
    {
        std::cerr << "Error! File cannot be opened." << std::endl;
        std::abort();
    }
    while (1){
        if (std::isspace(is.peek())) {
            is.get();
            continue;
        }
        else if (is.peek() == EOF) {
            break;
        }
        PktCards pc(is);
        std::cout << "Read '" << pc << "'" << std::endl;
    }
    
    return 0;
}
