#include<algorithm>
#include<types.h>
#include<dsConfig.h>
#include<HandRanker.h>
#include<Deck.h>
#include<PktCards.h>
#include<error.h>

using namespace ds;
int main() {
    int nIters = 10000;
    for (int i = 0; i < nIters; ++i) {
        Board bd;
        Deck dk;
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
        HandRanker::HandTypeStruct htsA(HandRanker::getHandType(bd, pktA));
        std::cout << pktA << ": "
            << HandRanker::HandTypeNames[htsA.ht] << " ["
            << Card::valueToWriteChar(htsA.values.first) << " "
            << Card::valueToWriteChar(htsA.values.second) << "] k["
            << Card::valueToWriteChar(htsA.kickers.first) << " "
            << Card::valueToWriteChar(htsA.kickers.second) << "]"
            << std::endl;
        if (res <= 0) {
            std::cout << " ** ";
        } else {
            std::cout << "    ";
        }
        HandRanker::HandTypeStruct htsB(HandRanker::getHandType(bd, pktB));
        std::cout << pktB << ": "
            << HandRanker::HandTypeNames[htsB.ht] << " ["
            << Card::valueToWriteChar(htsB.values.first) << " "
            << Card::valueToWriteChar(htsB.values.second) << "] k["
            << Card::valueToWriteChar(htsB.kickers.first) << " "
            << Card::valueToWriteChar(htsB.kickers.second) << "]"
            << std::endl;
        if (htsA.ht == htsB.ht && htsA.values == htsB.values) {
            std::cout << "^^^^^^^^^" << std::endl;
        }
    }
    return 0;
}