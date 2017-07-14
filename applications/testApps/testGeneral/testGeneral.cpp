#include <dsConfig.h>
#include <types.h>
#include <Deck.h>
#include <DeckMask.h>
#include <HandRanker.h>
#include <PktCards.h>

using namespace ds;

int main(int argc, char *argv[]) {
    PktCards pkt(Card::ace, Card::clubs, Card::ace, Card::hearts);
    Board bd;
    HandRanker hr(bd, pkt);
    std::vector<short> pa(hr.predictPreFlopToRiver());
    std::vector<int> hist(HandRanker::paToHistogram(pa));
    std::cout << "pa = [" << pa << "]\n";
    std::cout << "hist = [" << hist << "]" << std::endl;
    return 0;
}


// ****** END ****** //