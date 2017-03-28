#include <algorithm>
#include <dsConfig.h>
#include <handRanker.h>
#include <error.h>

using namespace ds;
int main() {
    short boardSize;
    std::cout << "How many distinct values are on the board? ";
    std::cin >> boardSize;
    valueSet vs(boardSize);
    valueSet::iterator it;
    for (it = vs.begin(); it != vs.end(); ++it) {
        std::cout << "Value " << it - vs.begin() + 1 << "? ";
        std::cin >> *it;
    }
    std::sort (vs.begin(), vs.end());
    std::cout << std::endl;    
    std::cout << vs[0];
    for (it = vs.begin() + 1; it != vs.end(); ++it) {
        std::cout << " " << *it;
    }
    std::cout << ":" << std::endl;
    straightCompleters sc(handRanker::findStraightCompleters(vs));
    valueSet::const_iterator scvIt;
    pocketValuesSet::const_iterator scpIt;
    for (
        scvIt = sc.first.begin(), scpIt = sc.second.begin();
        scvIt != sc.first.end();
        ++scvIt, ++scpIt
    ) {
        short maxVal = *scvIt;
        short card1 = scpIt->first;
        short card2 = scpIt->second;
        std::cout << "    " << maxVal << ": (" << card1 << " "
            << card2 << ")" << std::endl;
    }
    return 0;
}
