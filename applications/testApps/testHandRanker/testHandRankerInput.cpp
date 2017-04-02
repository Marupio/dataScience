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
//    std::cout << vs[0];
//    for (it = vs.begin() + 1; it != vs.end(); ++it) {
//        std::cout << " " << *it;
//    }
//    std::cout << ":" << std::endl;
    StraightCompleters sc(HandRanker::findStraightCompleters(vs));
    VecCardVal::const_iterator scvIt;
    VecPktVals::const_iterator scpIt;
    for (
        scvIt = sc.first.begin(), scpIt = sc.second.begin();
        scvIt != sc.first.end();
        ++scvIt, ++scpIt
    ) {
        CardVal maxVal = *scvIt;
        CardVal card1 = scpIt->first;
        CardVal card2 = scpIt->second;
        std::cout << "    " << maxVal << ": (" << card1 << " "
            << card2 << ")" << std::endl;
    }
    return 0;
}
