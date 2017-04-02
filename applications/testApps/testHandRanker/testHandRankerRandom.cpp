#include <algorithm>
#include <dsConfig.h>
#include <HandRanker.h>
#include <error.h>

using namespace ds;
int main() {
    int nIters = 100000;
    for (int i = 0; i < nIters; ++i) {
        short boardSize = 3 + std::rand()%2;
//        short boardSize = 5;
        VecCardVal vs(boardSize);
        bool duplicates = true;
        VecCardVal::iterator it;
        while (duplicates) {
            for (it = vs.begin(); it != vs.end(); ++it) {
                *it = std::rand()%13 + 2;
            }
            std::sort (vs.rbegin(), vs.rend());
            VecCardVal::iterator itNext;
            bool noDuplicates = true;
            for (
                it = vs.begin(), itNext = it+1;
                itNext != vs.end();
                ++it, ++itNext
            ) {
                if (*it == *itNext) {
                    noDuplicates = false;
                    break;
                }
            }
            if (noDuplicates) {
                duplicates = false;
            }
        }
        std::cout << i << ": " << vs[0];
        for (it = vs.begin() + 1; it != vs.end(); ++it) {
            std::cout << " " << *it;
        }
        std::cout << ":" << std::endl;
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
    }
    return 0;
}
