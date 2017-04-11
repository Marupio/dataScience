#ifndef HandRanker_h
#define HandRanker_h

#include<types.h>
#include<Board.h>
#include<PktCards.h>

namespace ds {

class HandRanker
{

public:

    // Public datatypes
    enum HandTypeEnum {
        HtStraightFlush,
        HtFoak,
        HtFullHouse,
        HtFlush,
        HtStraight,
        HtToak,
        HtTwoPair,
        HtPair,
        HtHighCard
    };

    struct HandTypeStruct {
        // Public data

            //- Type of hand (e.g. two-pair, high card, etc)
            HandTypeEnum ht;
            
            //- Card value associated with the hand type. Examples:
            //  * Straight - this is the highest card of the straight
            //  * Full house - this is the value of the set
            //  * Pair - this is the value of the pair
            CardVal value;

        // Constructors    

            //- Construct from components
            HandTypeStruct(HandTypeEnum t, CardVal v):
                ht(t),
                value(v)
            {}
    };

    //- Return the exact ranking of the pocket cards among all possibilities
    //  *** Located in HandRankerRank.cpp
    static short getRank(const Board& bd, const PktCards& pkt);

    //- Return the hand type for a given board and pocket
    //  *** Located in HandRankerGetHandType.cpp
    static HandTypeStruct getHandType(const Board& bd, const PktCards& pkt);

    //- Compare two pockets against the board
    //  Returns pktA > pktB
//    static bool compare
//    (
//        const Board& bd,
//        const PktCards& pktA,
//        const PktCards& pktB
//    );

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
