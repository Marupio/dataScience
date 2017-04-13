#include <algorithm>
#include <dsConfig.h>
#include <HandRanker.h>
#include <error.h>

using namespace ds;
int main() {
    short boardSize;
    std::cout << "How many distinct values are on the board? ";
    std::cin >> boardSize;
    VecCardVal vs(boardSize);
    VecCardVal::iterator it;
    for (it = vs.begin(); it != vs.end(); ++it) {
        std::cout << "Value " << it - vs.begin() + 1 << "? ";
        std::cin >> *it;
    }
    std::sort (vs.rbegin(), vs.rend());
    std::cout << std::endl;    
    std::cout << vs << std::endl;
    StraightCompleters sc(HandRanker::findStraightCompleters(vs));
    for (auto scvIt = sc.cbegin(); scvIt != sc.cend(); ++scvIt) {
        std::cout << "    " << scvIt->first << ": (" << scvIt->second.first
            << " " << scvIt->second.second << ")" << std::endl;
    }

/*    PktCards pkt;
    Board bd;
    std::cout << "First pocket card? ";
    std::cin >> pkt.first;
    std::cout << "Second pocket card? ";
    std::cin >> pkt.second;
    std::cout << "Board cards? ";
    std::cin >> bd;
    std::cout << bd << ", " << pkt << std::endl;

    const VecValStats& stats(bd.stats());
    VecCardVal values;
    values.reserve(stats.size());
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        values.push_back(it->value());
    }
    const StraightCompleters sc(
        HandRanker::findStraightCompleters(values)
    );
    for (auto it = sc.cbegin(); it != sc.cend(); ++it) {
        std::cout << it->first << ": [" << it->second.first << " "
            << it->second.second << "]" << std::endl;
    }

    HandRanker::HandTypeStruct hts(HandRanker::getHandType(bd, pkt));
    std::cout << HandRanker::HandTypeNames[hts.ht] << " ["
        << Card::valueToWriteChar(hts.values.first) << " "
        << Card::valueToWriteChar(hts.values.second) << "]" << std::endl;
*/
    return 0;
}
