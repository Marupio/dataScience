#include <algorithm>
#include <dsConfig.h>
#include <HandRanker.h>
#include <StraightCompleters.h>
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
    StraightCompleters sc(vs);
    for (auto scvIt = sc.cbegin(); scvIt != sc.cend(); ++scvIt) {
        std::cout << "    " << scvIt->first << ": (" << scvIt->second.first
            << " " << scvIt->second.second << ")" << std::endl;
    }
    return 0;
}
