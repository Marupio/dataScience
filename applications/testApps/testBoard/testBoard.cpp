#include<fstream>
#include<string>
#include<Board.h>
#include<Deck.h>
#include<dsConfig.h>
#include<types.h>
#include <HandRanker.h>

using namespace ds;

void writeDetails(const Board& bd)
{
    std::cout << "        Values: " << bd.cards() << std::endl;
    for (auto it = bd.stats().cbegin(); it != bd.stats().cend(); ++it) {
        std::cout << "              : " << it->value() << " ("
            << it->nCards() << ") ";
        writeSuitCount(it->suits(), std::cout);
        std::cout << "\n";
    }
    Suit fSuit(bd.flushSuit());
    const VecCardVal& fVals(bd.flushValues());
    CardVal bdFoak(bd.foak());
    Card bdSet(bd.set(), bd.setMissingSuit());
    PktCards bdPairA(
        bd.pairA(), bd.pairAMissingSuits().first,
        bd.pairA(), bd.pairAMissingSuits().second
    );
    PktCards bdPairB(
        bd.pairB(), bd.pairBMissingSuits().first,
        bd.pairB(), bd.pairBMissingSuits().second
    );
    std::cout << "          FOAK: " << Card::valueToWriteChar(bdFoak);
    std::cout << "\n           Set: " << bdSet;
    std::cout << "\n         PairA: " << bdPairA;
    std::cout << "\n         PairB: " << bdPairB;
    std::cout << "\n    Flush suit: " << Card::suitToWriteChar(fSuit);
    std::cout << "\n   Flush values: " << fVals;

    std::cout << "\n\n      lowestVal: "
        << Card::valueToWriteChar(bd.lowestValue());
    std::cout << "\n   lowestVal(2): "
        << Card::valueToWriteChar(bd.lowestValue(2));
    std::cout << "\n lowestVal(2,3): "
        << Card::valueToWriteChar(bd.lowestValue(PktVals(2,3)));

    std::cout << "\n\n      lowestTwo: "
        << bd.lowestTwoValues();
    std::cout << "\n   lowestTwo(2): "
        << bd.lowestTwoValues(2);
    std::cout << "\n lowestTwo(2,3): "
        << bd.lowestTwoValues(PktVals(2,3));

    std::cout << "\n\n     highestVal: "
        << Card::valueToWriteChar(bd.highestValue());
    std::cout << "\n  highestVal(A): "
        << Card::valueToWriteChar(bd.highestValue(14));
    std::cout << "\nhighestVal(A,K): "
        << Card::valueToWriteChar(bd.highestValue(PktVals(14,13)));

    std::cout << "\n\n     highestTwo: "
        << bd.highestTwoValues();
    std::cout << "\n  highestTwo(A): "
        << bd.highestTwoValues(14);
    std::cout << "\nhighestTwo(A,K): "
        << bd.highestTwoValues(PktVals(14,13));

    if (fVals.size()) {
        std::cout << "\n\n   lowestFlush: "
            << bd.lowestFlushValue();
        std::cout << "\n  lowest2Flush: "
            << bd.lowestTwoFlushValues();
        std::cout << "\n\n  highestFlush: "
            << bd.highestFlushValue();
    }

    std::cout << std::endl;
}


int main()
{
    int nIters = 1000;
    for (int i = 0; i < nIters; ++i) {
        Board bd;
        Deck dk;
        dk.shuffle();
        
        std::cout << i << ":\n    Empty: " << bd << "\n";
        {
//            writeDetails(bd);
        }
        std::cout << std::endl;
        
        bd.flop(dk.draw(3));
//        {
//            VecDeckInd di(3);
//            VecDeckInd::iterator it;
//            for (it = di.begin(); it != di.end(); ++it) {
//                *it = std::rand()%52;
//            }
//            bd.flop(di);
//        }

        std::cout << ":\n    Flop: " << bd << "\n";
        {
//            writeDetails(bd);
        }
        std::cout << std::endl;

//        bd.turn(std::rand()%52);
        bd.turn(dk.draw());        
        std::cout << ":\n    Turn: " << bd << "\n";
        {
//            writeDetails(bd);
        }
        std::cout << std::endl;

//        bd.river(std::rand()%52);
        bd.river(dk.draw());
        std::cout << ":\n    River: " << bd << "\n";
        {
            writeDetails(bd);
        }
        std::cout << "\n---------------------------------------\n" << std::endl;
    }
    return 0;
}
