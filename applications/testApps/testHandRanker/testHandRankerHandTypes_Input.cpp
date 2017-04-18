#include <algorithm>
#include <dsConfig.h>
#include <HandRanker.h>
#include <StraightCompleters.h>
#include <error.h>

using namespace ds;
int main() {
    PktCards pkt;
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
    const StraightCompleters sc(values);
    for (auto it = sc.cbegin(); it != sc.cend(); ++it) {
        std::cout << it->first << ": [" << it->second.first << " "
            << it->second.second << "]" << std::endl;
    }

    HandType hts(bd, pkt);
    std::cout << HandType::HandTypeNames[hts.ht()] << " ["
        << Card::valueToWriteChar(hts.values().first) << " "
        << Card::valueToWriteChar(hts.values().second) << "]" << std::endl;

    return 0;
}
