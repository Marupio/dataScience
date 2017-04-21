#include<algorithm>
#include<types.h>
#include<dsConfig.h>
#include<HandRanker.h>
#include<Deck.h>
#include<PktCards.h>
#include<error.h>

using namespace ds;
int main() {
    int nIters = 10000;
    for (int i = 0; i < nIters; ++i) {
        Board bd;
        Deck dk;
        dk.shuffle();
        PktCards pkt(dk.draw(2));
        bd.flop(dk.draw(3));
        bd.turn(dk.draw());
        bd.river(dk.draw());
        HandRanker hr(bd, pkt);
        short rank = hr.rank();
        std::cout << i << ": " << bd.cards() << " " << pkt << " = " << rank
            << std::endl;
    }
    return 0;
}
