#ifndef HandRanker_h
#define HandRanker_h

#include<string>
#include<types.h>
#include<Board.h>
#include<PktCards.h>
#include<StraightCompleters.h>

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

    struct HandType {
        // Public data

            //- Type of hand (e.g. two-pair, high card, etc)
            char ht;
            
            //- Card value associated with the hand type. Examples:
            //  * Straight - this is the highest card of the straight
            //  * Full house - this is the value of the set
            //  * Pair - this is the value of the pair
            PktVals values;

        // Constructors    

            //- Construct from components
            HandType(char t, PktVals v);

            //- Construct from components
            HandType(char t, CardVal vA, CardVal vB);
    };


    // Public member functions

        //- Return the exact ranking of the pocket cards among all possibilities
        //  *** Located in HandRankerRank.cpp
        static short getRank(const Board& bd, const PktCards& pkt);

        //- Return the hand type for a given board and pocket
        //  *** Located in HandRankerGetHandType.cpp
        static HandType getHandType(const Board& bd, const PktCards& pkt);

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
