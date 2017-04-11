#include<algorithm>
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
        PktCards pkt(dk.draw(2));
        bd.flop(dk.draw(3));
        bd.turn(dk.draw());
        bd.river(dk.draw());
        std::cout << i << ": " << bd.cards() << " " << pkt << std::endl;
        HandRanker::HandTypeStruct hts(HandRanker::getHandType(bd, pkt));
        switch (hts.ht){
        case HandRanker::HtStraightFlush: {
            std::cout << "    Straight flush, "
                << Card::valueToWriteChar(hts.value) << std::endl;
            break;
        }
        case HandRanker::HtFoak: {
            std::cout << "    Four-of-a-kind, "
                << Card::valueToWriteChar(hts.value) << std::endl;
            break;
        }
        case HandRanker::HtFullHouse: {
            std::cout << "    Full house, "
                << Card::valueToWriteChar(hts.value) << std::endl;
            break;
        }
        case HandRanker::HtFlush: {
            std::cout << "    Flush, "
                << Card::valueToWriteChar(hts.value) << std::endl;
            break;
        }
        case HandRanker::HtStraight: {
            std::cout << "    Straight, "
                << Card::valueToWriteChar(hts.value) << std::endl;
            break;
        }
        case HandRanker::HtToak: {
            std::cout << "    Three-of-a-kind, "
                << Card::valueToWriteChar(hts.value) << std::endl;
            break;
        }
        case HandRanker::HtTwoPair: {
            std::cout << "    Two pair, "
                << Card::valueToWriteChar(hts.value) << std::endl;
            break;
        }
        case HandRanker::HtPair: {
            std::cout << "    Pair, "
                << Card::valueToWriteChar(hts.value) << std::endl;
            break;
        }
        case HandRanker::HtHighCard: {
            std::cout << "    High card, "
                << Card::valueToWriteChar(hts.value) << std::endl;
            break;
        }
        default: {
        } // end default
        } // end switch
    }
    return 0;
}
