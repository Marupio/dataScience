#include<fstream>
#include<string>
#include<Board.h>
#include<dsConfig.h>
#include<types.h>

using namespace ds;
int main()
{
    int nIters = 10;
    for (int i = 0; i < nIters; ++i) {
        Board b1;
        
        std::cout << i << ":\n    Empty: " << b1 << "\n";
        {
            const VecCardVal& b1Suv(b1.sortedUniqueVals());
            const std::vector<short>& b1SuvCounts(b1.sortedUniqueValCounts());
            const std::vector<short>& b1SuitCounts(b1.suitCounts());
            std::cout << "        Values: ";
            for (short i = 0; i < b1Suv.size(); ++i) {
                std::cout << b1Suv[i] << "(" << b1SuvCounts[i] << ") ";
            }
            std::cout << "\n         Suits: ";
            for (short i = 0; i < 4; ++i) {
                std::cout << Card::suitToWriteChar(i) << "(" << b1SuitCounts[i]
                    << ") ";
            }
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
            const VecCardVal& b1Suv(b1.sortedUniqueVals());
            const std::vector<short>& b1SuvCounts(b1.sortedUniqueValCounts());
            const std::vector<short>& b1SuitCounts(b1.suitCounts());
            std::cout << "        Values: ";
            for (short i = 0; i < b1Suv.size(); ++i) {
                std::cout << b1Suv[i] << "(" << b1SuvCounts[i] << ") ";
            }
            std::cout << "\n         Suits: ";
            for (short i = 0; i < 4; ++i) {
                std::cout << Card::suitToWriteChar(i) << "(" << b1SuitCounts[i]
                    << ") ";
            }
        }
        std::cout << std::endl;

        b1.turn(std::rand()%52);
        std::cout << ":\n    Turn: " << b1 << "\n";
        {
            const VecCardVal& b1Suv(b1.sortedUniqueVals());
            const std::vector<short>& b1SuvCounts(b1.sortedUniqueValCounts());
            const std::vector<short>& b1SuitCounts(b1.suitCounts());
            std::cout << "        Values: ";
            for (short i = 0; i < b1Suv.size(); ++i) {
                std::cout << b1Suv[i] << "(" << b1SuvCounts[i] << ") ";
            }
            std::cout << "\n         Suits: ";
            for (short i = 0; i < 4; ++i) {
                std::cout << Card::suitToWriteChar(i) << "(" << b1SuitCounts[i]
                    << ") ";
            }
        }
        std::cout << std::endl;

        b1.river(std::rand()%52);
        std::cout << ":\n    River: " << b1 << "\n";
        {
            const VecCardVal& b1Suv(b1.sortedUniqueVals());
            const std::vector<short>& b1SuvCounts(b1.sortedUniqueValCounts());
            const std::vector<short>& b1SuitCounts(b1.suitCounts());
            std::cout << "        Values: ";
            for (short i = 0; i < b1Suv.size(); ++i) {
                std::cout << b1Suv[i] << "(" << b1SuvCounts[i] << ") ";
            }
            std::cout << "\n         Suits: ";
            for (short i = 0; i < 4; ++i) {
                std::cout << Card::suitToWriteChar(i) << "(" << b1SuitCounts[i]
                    << ") ";
            }
        }
        std::cout << std::endl;
    }
    return 0;
}
