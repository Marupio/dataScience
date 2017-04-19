#ifndef HandRanker_h
#define HandRanker_h

#include<types.h>
#include<Board.h>
#include<PktCards.h>
#include<StraightCompleters.h>
#include<HandType.h>
#include<PktMask.h>

namespace ds {

class HandRanker
{

public:

    // Constructors
    
        //- Construct from board and pocket
        HandRanker(const Board& bd, const PktCards& pkt);


    // Public member functions

        //- Return the exact ranking of the pocket cards among all possibilities
        //  *** Located in HandRankerRank.cpp
        short getRank();

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

    // Private Member Functions
    
        //- Helper for compare
        //  Finds kickers, if any, given the provided hand type
        //  ** Located in HandRankerGetKickers.cpp
        static PktVals getKickers(
            const Board& bd,
            const PktCards& pkt,
            const HandType& ht
        );

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
    
        //- Reference to the board
        const Board& bd_;
        
        //- Reference to the pocket
        const PktCards& pkt_;
        
        //- Base pocket mask - only has board and pocket removed, used to reset
        //  pocketMask for iterating over future possibilities
        PktMask baseMask_;
        
        //- Working pocket mask
        PktMask mask_;

};

}

#include<HandRankerRankKickers.cpp>

#endif
