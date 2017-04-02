#include<fstream>
#include<string>
#include<Board.h>
#include<dsConfig.h>
#include<types.h>

using namespace ds;

void writeDetails(const Board(b1))
{
    const VecCardVal& b1v(b1.values());
    const std::vector<short>& b1vCounts(b1.valueCounts());
    const VecVecSuit& b1vSuits(b1.valueSuits());
    const SuitCount& b1SuitCounts(b1.suitCounts());
    Suit fSuit(b1.flushSuit());
    const VecCardVal& fVals(b1.flushVals());
    CardVal b1Foak(b1.foak());
    CardVal b1Toak(b1.toak());
    const PktVals& b1Pairs(b1.pairs());

    std::cout << "        Values: " << b1v;
    std::cout << "\n  Value counts: (";
    if (b1v.size()) {
        std::cout << b1vCounts[0];
    }
    for (short i = 1; i < b1v.size(); ++i) {
        std::cout << " " << b1vCounts[i];
    }
    std::cout << ")\n         Suits: ";
    writeVecVecSuit(b1vSuits, std::cout);
    std::cout << "   Suit counts: ";
    writeSuitCount(b1SuitCounts, std::cout);
    std::cout << "\n    Flush suit: " << Card::suitToWriteChar(fSuit);
    std::cout << "\n   Flush values: " << fVals;
    std::cout << "\n Four of a kind: "
        << Card::valueToWriteChar(b1Foak);
    std::cout << "\nThree of a kind: "
        << Card::valueToWriteChar(b1Toak);
    std::cout << "\n          Pairs: ["
        << Card::valueToWriteChar(b1Pairs.first) << " "
        << Card::valueToWriteChar(b1Pairs.second) << "]";
}


int main()
{
    int nIters = 100000;
    for (int i = 0; i < nIters; ++i) {
        Board b1;
        
        std::cout << i << ":\n    Empty: " << b1 << "\n";
        {
//            writeDetails(b1);
        }
        std::cout << std::endl;
        
        {
            VecDeckInd di(3);
            VecDeckInd::iterator it;
            for (it = di.begin(); it != di.end(); ++it) {
                *it = std::rand()%52;
            }
            b1.flop(di);
        }

        std::cout << ":\n    Flop: " << b1 << "\n";
        {
//            writeDetails(b1);
        }
        std::cout << std::endl;

        b1.turn(std::rand()%52);
        std::cout << ":\n    Turn: " << b1 << "\n";
        {
//            writeDetails(b1);
        }
        std::cout << std::endl;

        b1.river(std::rand()%52);
        std::cout << ":\n    River: " << b1 << "\n";
        {
            writeDetails(b1);
        }
        std::cout << "\n---------------------------------------\n" << std::endl;
    }
    return 0;
}
