#include <Card.h>
#include <Deck.h>
#include <EntropyInterface.h>

using namespace ds;

int main(int argc, char const *argv[])
{
    EntropyInterface ent;
    Deck dk(ent, "testDeck");
    size_t n = 10;
    while (n--) {
        dk.shuffle();
        std::cout << n << ": ";
        size_t i = 52;
        while(i--) {
            DeckInd di = dk.draw();
            Card drawn(di);
            std::cout << " " << drawn;
        }
        std::cout << std::endl;

    }
    return 0;
}