#include<algorithm>
#include<types.h>
#include<dsConfig.h>
#include<HandRanker.h>
#include<Deck.h>
#include<DeckMask.h>
#include<PktCards.h>
#include<error.h>

using namespace ds;
int main() {
    int nIters = 1000;
    VecDeckInd::size_type arraySizes(1081);
    for (int i = 0; i < nIters; ++i) {
        Board bd;
        Deck dk;
        dk.shuffle();
        bd.flop(dk.draw(3));
        bd.turn(dk.draw());
        bd.river(dk.draw());
        std::cout << i << ": " << bd << ":" << std::endl;
        DeckMask dm(bd);

        std::vector<short> rankArray;
        rankArray.reserve(arraySizes);
        VecPktCards pktCardsArray;
        pktCardsArray.reserve(arraySizes);
        
        for (auto itA = dm.cbegin(); itA != dm.cend(); ++itA) {
            for (auto itB = itA + 1; itB != dm.cend(); ++itB) {
                pktCardsArray.push_back(PktCards(*itA, *itB));
                HandRanker hr(bd, pktCardsArray.back(), false);
                rankArray.push_back(hr.rank());
            }
        }
        // Custom sorting
        std::vector<size_t> sorted(arraySizes);
        std::iota(sorted.begin(), sorted.end(), 0);
        std::sort(
            sorted.begin(),
            sorted.end(),
            [&rankArray](size_t i1, size_t i2)
                {return rankArray[i1] < rankArray[i2];}
        );
        if (sorted.size()) {
            for (auto it = sorted.cbegin(); (it + 1) != sorted.cend(); ++it) {
                size_t iA = *it;
                size_t iB = *(it + 1);
                const PktCards& pktA = pktCardsArray[iA];
                const PktCards& pktB = pktCardsArray[iB];
                short rankA = rankArray[iA];
                short rankB = rankArray[iB];
                short comp = HandRanker::compare(bd, pktA, pktB);
                std::cout << "    " << pktA << " (" << rankA << ") ";
                if (comp > 0) {
                    std::cout << ">";
                } else if (comp == 0) {
                    std::cout << "=";
                } else if (comp < 0) {
                    std::cout << "<";
                }
                std::cout << " " << pktB << " (" << rankB << ") " << std::endl;
            }
        }
    }
    return 0;
}
