#include <algorithm>
#include <dsConfig.h>
#include <HandRanker.h>
#include <error.h>

using namespace ds;
int main() {
    PktCards pktA;
    PktCards pktB;
    Board bd;
    std::cout << "Player 1 pocket cards? ";
    std::cin >> pktA;
    std::cout << "Player 2 pocket cards? ";
    std::cin >> pktB;
    std::cout << "Board cards? ";
    std::cin >> bd;
    std::cout << bd << ":" << std::endl;

    HandType htA(bd, pktA);
    HandType htB(bd, pktB);
    short comp = HandRanker::compare(bd, pktA, pktB);
    if (comp >= 0) {
        std::cout << " ** ";
    } else {
        std::cout << "    ";
    }
    std::cout << pktA << ": "
        << HandType::HandTypeNames[htA.ht()] << " ["
        << Card::valueToWriteChar(htA.values().first) << " "
        << Card::valueToWriteChar(htA.values().second) << "]"
        << std::endl;
    if (comp <= 0) {
        std::cout << " ** ";
    } else {
        std::cout << "    ";
    }
    std::cout << pktB << ": "
        << HandType::HandTypeNames[htB.ht()] << " ["
        << Card::valueToWriteChar(htB.values().first) << " "
        << Card::valueToWriteChar(htB.values().second) << "]"
        << std::endl;

    return 0;
}
