#ifndef HandRanker_h
#define HandRanker_h

#include<string>
#include<types.h>
#include<Board.h>
#include<PktCards.h>

namespace ds {

class HandRanker
{

public:

    // Public static data

        //- Hand types
        static const char HtUnknown;          // 0
        static const char HtHighCard;         // 1
        static const char HtPair;             // 2
        static const char HtTwoPair;          // 3
        static const char HtSet;              // 4
        static const char HtStraight;         // 5
        static const char HtFlush;            // 6
        static const char HtFullHouse;        // 7
        static const char HtFoak;             // 8
        static const char HtStraightFlush;    // 9

        //- Names associated with each Ht char
        static const std::array<std::string, 10> HandTypeNames;


    // Public data types

    struct HandTypeStruct {
        // Public data

            //- Type of hand (e.g. two-pair, high card, etc)
            char ht;
            
            //- Card value associated with the hand type. Examples:
            //  * Straight - this is the highest card of the straight
            //  * Full house - this is the value of the set
            //  * Pair - this is the value of the pair
            PktVals values;

            //- Kickers from pocket, if any
            PktVals kickers;

        // Constructors    

            //- Construct from components
            HandTypeStruct(char t, PktVals v, PktVals k):
                ht(t),
                values(v),
                kickers(k)
            {}

            //- Construct from components
            HandTypeStruct(char t, CardVal vA, CardVal vB, PktVals k):
                ht(t),
                values(vA, vB),
                kickers(k)
            {}

            //- Construct from components
            HandTypeStruct(char t, PktVals v, CardVal kA, CardVal kB):
                ht(t),
                values(v),
                kickers(kA, kB)
            {}

            //- Construct from components
            HandTypeStruct(
                char t,
                CardVal vA,
                CardVal vB,
                CardVal kA,
                CardVal kB
            ):
                ht(t),
                values(vA, vB),
                kickers(kA, kB)
            {}
    };


    // Public member functions

        //- Return the exact ranking of the pocket cards among all possibilities
        //  *** Located in HandRankerRank.cpp
        static short getRank(const Board& bd, const PktCards& pkt);

        //- Return the hand type for a given board and pocket
        //  *** Located in HandRankerGetHandType.cpp
        static HandTypeStruct getHandType(const Board& bd, const PktCards& pkt);

        //- Compare two pockets against the board
        //  Returns:
        //  -1: pktA < pktB
        //   0: pktA == pktB
        //   1: pktA > pktB
        static short compare
        (
            const Board& bd,
            const PktCards& pktA,
            const PktCards& pktB
        );

        //- Find pocket cards that complete a straight for a given set of values
        //  Values must be sorted and unique, it doesn't check
        //  ** Located in HandRankerFindStraightCompleters.cpp
        static StraightCompleters findStraightCompleters
        (
            const VecCardVal& values
        );


private:

    //- Helper function for findStraightCompleters
    //  ** Located in HandRankerFindStraightCompleters.cpp
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

    //- Helper for compare
    //  Finds two lowest cards on board that are not in avoid
    //  Checks if pktA or pktB can beat it, in the required suit
    //  Set one or both components of avoid to (unknownCard) when not required
    //  Set requiredSuit to unknownSuit when not required
    //  Set checkSecond to true to check both kickers
    //  *** Located in HandRankerCompare.cpp
    static short findAndCompareKickers
    (
        const Board& bd,
        const PktVals& avoid,
        Suit requiredSuit,
        bool checkSecond,
        const PktCards& pktA,
        const PktCards& pktB
    );

    //- Helper function for findAndCompareKickers
    //  Looks at pocket for higher kickers than those on board
    //  *** Located in HandRankerCompare.cpp
    static PktVals findTwoKickers(
        const PktCards& pkt,
        const PktVals& avoid,
        CardVal bdLowKicker,
        CardVal bdHighKicker,
        Suit requiredSuit
    );

    //- Helper function for findAndCompareKickers
    //  Compares kickers
    //  *** Located in HandRankerCompare.cpp
    static short checkKickers
    (
        const PktVals& kickersA,
        const PktVals& kickersB,
        bool checkSecond
    );
};

}
#endif
