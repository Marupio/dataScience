#include<algorithm>
#include<fstream>
#include<types.h>
#include<dsConfig.h>
#include<HandRanker.h>
#include<Deck.h>
#include<DeckMask.h>
#include<PktCards.h>
#include<error.h>

using namespace ds;

short checkAllCombinations(int i, const Board& bd, std::ostream& os) {
    short maxRank = 0;
    VecDeckInd::size_type arraySizes(1326);
    DeckMask dm(bd);

    std::vector<short> rankArray;
    rankArray.reserve(arraySizes);
    VecPktCards pktCardsArray;
    pktCardsArray.reserve(arraySizes);

    for (auto itA = dm.cbegin(); itA != dm.cend(); ++itA) {
        for (auto itB = itA + 1; itB != dm.cend(); ++itB) {
            pktCardsArray.push_back(PktCards(*itA, *itB));
            HandRanker hr(bd, pktCardsArray.back());
            rankArray.push_back(hr.rank());
        }
    }
    // Custom sorting
    std::vector<size_t> sorted(pktCardsArray.size());
    std::iota(sorted.begin(), sorted.end(), 0);
    std::sort(
        sorted.begin(),
        sorted.end(),
        [&rankArray](size_t i1, size_t i2)
            {return rankArray[i1] < rankArray[i2];}
    );
    if (sorted.size()) {
        maxRank = rankArray.front();
        for (auto it = sorted.cbegin(); (it + 1) != sorted.cend(); ++it) {
            size_t iA = *it;
            size_t iB = *(it + 1);
            const PktCards& pktA = pktCardsArray[iA];
            const PktCards& pktB = pktCardsArray[iB];
            short rankA = rankArray[iA];
            short rankB = rankArray[iB];
            maxRank = std::max(rankB, maxRank);
            short comp = HandRanker::compare(bd, pktA, pktB);
            if (comp < 0) {
                os << i << " - " << bd << ": " << pktA << " (" << rankA
                    << ") < " << pktB << " (" << rankB << ")" << std::endl;
            }
        }
    }
    return maxRank;
}

int main() {
    int nIters = 1000000;
    std::ofstream os("testHandRankerRankInfinitum_output");
    short maxRankFlop = 0;
    short maxRankTurn = 0;
    short maxRankRiver = 0;
    for (int i = 0; i < nIters; ++i) {
        if (i%1000 == 0) {
            std::cout << i << ": " << maxRankFlop << " " << maxRankTurn 
                << " " << maxRankRiver << std::endl;
        }
        Board bd;
        Deck dk;
        dk.shuffle();
        bd.flop(dk.draw(3));
        maxRankFlop = std::max(maxRankFlop, checkAllCombinations(i, bd, os));
        bd.turn(dk.draw());
        maxRankTurn = std::max(maxRankTurn, checkAllCombinations(i, bd, os));
        bd.river(dk.draw());
        maxRankRiver = std::max(maxRankRiver, checkAllCombinations(i, bd, os));
    }
    return 0;
}
