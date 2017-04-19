#include <algorithm>
#include <dsConfig.h>
#include <HandRanker.h>
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

    HandRanker hr(bd, pkt);
    short rank = hr.getRank();
    std::cout << "\n" << bd << " " << pkt << " = " << rank << std::endl;
    return 0;
}
