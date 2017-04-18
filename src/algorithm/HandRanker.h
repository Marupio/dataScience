#ifndef HandRanker_h
#define HandRanker_h

#include<types.h>
#include<Board.h>
#include<PktCards.h>
#include<StraightCompleters.h>
#include<HandType.h>

namespace ds {

class HandRanker
{

public:

    // Public member functions

        //- Return the exact ranking of the pocket cards among all possibilities
        //  *** Located in HandRankerRank.cpp
        static short getRank(const Board& bd, const PktCards& pkt);

        //- Compare two pockets against the board
        //  Returns:
        //  -1: pktA < pktB
        //   0: pktA == pktB
        //   1: pktA > pktB
        static short compare(
            const Board& bd,
            const PktCards& pktA,
            const PktCards& pktB
        );


private:

    //- Helper for compare
    //  ** Located in HandRankerGetKickers.cpp
    static PktVals getKickers(
        const Board& bd,
        const PktCards& pkt,
        const HandType& ht
    );
};

}
#endif
