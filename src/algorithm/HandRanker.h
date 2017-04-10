#ifndef HandRanker_h
#define HandRanker_h

#include<types.h>
#include<Board.h>
#include<PktCards.h>

namespace ds {

class HandRanker
{

public:

    //- Return the exact ranking of the pocket cards among all possibilities
    //  *** Located in HandRankerRank.cpp
    static short getRank(const Board& bd, const PktCards& pkt);

    //- Find pocket cards that complete a straight for a given set of values
    //  Values must be sorted and unique, it doesn't check
    static StraightCompleters findStraightCompleters(const VecCardVal& values);


private:

    //- Helper function for findStraightCompleters
    static bool restart(
        const VecCardVal& values,
        short& cursor,
        short& restartCursor,
        VecCardVal::const_iterator& vsIter,
        VecCardVal::const_iterator& altVsIter,
        short& curStraightSize,
        CardVal& curStraightMax,
        PktVals& pocket
    );

};

}
#endif
