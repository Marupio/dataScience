#ifndef HandRanker_h
#define HandRanker_h

#include <types.h>
#include <Board.h>
#include <PktCards.h>
#include <StraightCompleters.h>
#include <HandType.h>
#include <PktMask.h>

namespace ds {

class HandRanker
{

public:

    // Constructors
    
        //- Construct from board and pocket
        //  includePktInMask allows ranking to eliminate possible hand
        //  combinations that include one of the pkt cards.  This gives a more
        //  accurate ranking.
        //      For example:
        //      Board (A A 3 7 9), pkt is [A7]
        //      Looking at the board alone, the best hand is quad aces, but
        //      pkt proves there is no quad aces.
        //  However, including this information makes it harder to test the 
        //  ranking algorithm by comparing possible hands.
        HandRanker(
            const Board& bd,
            const PktCards& pkt,
            bool includePktInMask = true
        );


    // Public member functions

        //- Compare two pockets against the board
        //  Returns:
        //      -1: pktA < pktB
        //       0: pktA == pktB
        //       1: pktA > pktB
        //  ** Located in HandRankerCompare.cpp
        static short compare(
            const Board& bd,
            const PktCards& pktA,
            const PktCards& pktB
        );

        //- Return the exact ranking of the pocket cards among all possibilities
        //  *** Located in HandRankerRank.cpp
        short rank();

        //- Return array of rank values based on what the next card might be
        //  Suitable only after the flop and before the river.
        void predict(std::vector<short>& pa);

        //- Convert postgres array to vector of values
        static std::vector<short> readPredictString(std::string& pa);

        //- Return array of rank values based on what the flop might be
        //  Suitable only when the board is empty
        void predictFlop(std::vector<short>& pa);

        //- Helper for compare
        //  Finds kickers, if any, given the provided hand type
        //  ** Located in HandRankerGetKickers.cpp
        static PktVals getKickers(
            const Board& bd,
            const PktCards& pkt,
            const HandType& ht
        );


private:

    // Private Member Functions

        //- Ranks one kicker for hands where one kicker is possible
        //  ** Located in HandRankerRankKickers
        template <typename Avoider>
        void rankOneKicker(
            short& rank,
            const VecCard& reqdPktFirstCards,
            CardVal bdKicker,
            Avoider avoid
        );

        //- Ranks two kickers for hands where two kickers are possible
        //  ** Located in HandRankerRankKickers
        template <typename Avoider>
        void rankTwoKickers(
            short& rank,
            const PktVals& bdKickers,
            Avoider avoid
        );

        //- Ranks hands by suited kickers for a three flush board
        //  ** Located in HandRankerKickers
        void rankKickersThreeFlush(short& rank);

        //- Ranks hands by suited kickers for a four flush board
        //  ** Located in HandRankerKickers
        void rankKickersFourFlush(short& rank);

        //- Ranks hands by suited kickers for a five flush board
        //  ** Located in HandRankerKickers
        void rankKickersFiveFlush(short& rank);


    // Private Member Data
    
        //- Reference to the pocket
        const PktCards& pkt_;
        
        //- Reference to the board
        const Board& bd_;
        
        //- Working copy of the board
        Board cbd_;
        
        //- Base pocket mask - only has board and pocket removed, used to reset
        //  pocketMask for iterating over future possibilities
        PktMask baseMask_;
        
        //- Working pocket mask
        PktMask mask_;

};

}

#endif
