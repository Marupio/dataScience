#include <fstream>
#include <Board.h>
#include <Deck.h>
#include <dsConfig.h>
#include <types.h>

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
    {
        std::cout << "Test perfect flop hash" << std::endl;
        VecDeckInd vdi;
        vdi.reserve(3);
        vdi.push_back(51);
        vdi.push_back(50);
        vdi.push_back(49);
        Board bdTestAFromVdi(vdi);
        int hashA = bdTestAFromVdi.perfectFlopHash();
        std::cout << hashA << ":" << bdTestAFromVdi << std::endl;
        Board bdTestAFromHash(Board::makeBoardFromFlopHash(hashA));
        std::cout << bdTestAFromHash << std::endl;

        vdi[0] = 2;
        vdi[1] = 1;
        vdi[2] = 0;

        Board bdTestBFromVdi(vdi);
        int hashB = bdTestBFromVdi.perfectFlopHash();
        std::cout << bdTestBFromVdi.perfectFlopHash() << ":" << bdTestBFromVdi << std::endl;
        Board bdTestBFromHash(Board::makeBoardFromFlopHash(hashB));
        std::cout << bdTestBFromHash << std::endl;
    }
    {
        std::cout << "Test perfect turn hash" << std::endl;
        VecDeckInd vdi;
        vdi.reserve(4);
        vdi.push_back(51);
        vdi.push_back(50);
        vdi.push_back(49);
        vdi.push_back(48);
        Board bdTestAFromVdi(vdi);
        int hashA = bdTestAFromVdi.perfectTurnHash();
        std::cout << hashA << ":" << bdTestAFromVdi << std::endl;
        Board bdTestAFromHash(Board::makeBoardFromTurnHash(hashA));
        std::cout << bdTestAFromHash << std::endl;

        vdi[0] = 3;
        vdi[1] = 2;
        vdi[2] = 1;
        vdi[3] = 0;

        Board bdTestBFromVdi(vdi);
        int hashB = bdTestBFromVdi.perfectTurnHash();
        std::cout << bdTestBFromVdi.perfectTurnHash() << ":" << bdTestBFromVdi << std::endl;
        Board bdTestBFromHash(Board::makeBoardFromTurnHash(hashB));
        std::cout << bdTestBFromHash << std::endl;
    }
    {
        std::cout << "Test perfect river hash" << std::endl;
        VecDeckInd vdi;
        vdi.reserve(5);
        vdi.push_back(51);
        vdi.push_back(50);
        vdi.push_back(49);
        vdi.push_back(48);
        vdi.push_back(47);
        Board bdTestAFromVdi(vdi);
        int hashA = bdTestAFromVdi.perfectRiverHash();
        std::cout << hashA << ":" << bdTestAFromVdi << std::endl;
        Board bdTestAFromHash(Board::makeBoardFromRiverHash(hashA));
        std::cout << bdTestAFromHash << std::endl;

        vdi[0] = 4;
        vdi[1] = 3;
        vdi[2] = 2;
        vdi[3] = 1;
        vdi[4] = 0;

        Board bdTestBFromVdi(vdi);
        int hashB = bdTestBFromVdi.perfectRiverHash();
        std::cout << bdTestBFromVdi.perfectRiverHash() << ":" << bdTestBFromVdi << std::endl;
        Board bdTestBFromHash(Board::makeBoardFromRiverHash(hashB));
        std::cout << bdTestBFromHash << std::endl;
    }

    int nIters = 5;
    for (int i = 0; i < nIters; ++i) {
        Board bd;
        EntropyInterface ent;
        Deck dk(ent, "testBoard");
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
