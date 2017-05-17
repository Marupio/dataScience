#include <algorithm>
#include <types.h>
#include <dsConfig.h>
#include <HandRanker.h>
#include <Deck.h>
#include <PktCards.h>
#include <error.h>

using namespace ds;
int main() {
    int nIters = 10000;
    for (int i = 0; i < nIters; ++i) {
        Board bd;
        EntropyInterface ent;
        Deck dk(ent, "testHandRankerCompare_Random");
        dk.shuffle();
        PktCards pktA(dk.draw(2));
        PktCards pktB(dk.draw(2));
        bd.flop(dk.draw(3));
        bd.turn(dk.draw());
        bd.river(dk.draw());
        std::cout << i << ": " << bd.cards() << std::endl;
        short res = HandRanker::compare(bd, pktA, pktB);
        if (res >= 0) {
            std::cout << " ** ";
        } else {
            std::cout << "    ";
        }
        HandType htsA(bd, pktA);
        std::cout << pktA << ": "
            << HandType::HandTypeNames[htsA.ht()] << " ["
            << Card::valueToWriteChar(htsA.values().first) << " "
            << Card::valueToWriteChar(htsA.values().second) << "]"
            << std::endl;
        if (res <= 0) {
            std::cout << " ** ";
        } else {
            std::cout << "    ";
        }
        HandType htsB(bd, pktB);
        std::cout << pktB << ": "
            << HandType::HandTypeNames[htsB.ht()] << " ["
            << Card::valueToWriteChar(htsB.values().first) << " "
            << Card::valueToWriteChar(htsB.values().second) << "]"
            << std::endl;
        if (htsA.ht() == htsB.ht() && htsA.values() == htsB.values()) {
            std::cout << "^^^^^^^^^" << std::endl;
        }
    }
    return 0;
}
