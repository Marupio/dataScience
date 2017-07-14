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

    // Public static data

        //- Starting hand river rank histogram
        //  Associated index is PktCards::startingHandIndex()
        const static std::vector<std::vector<int>> shrrHist_;

        //- Maximum (worst) river rank value = 1325
        //  Practically speaking, the worst river rank is actually 1225, but there are 1325
        //  possible hands opponents could have
        const static short maxRiverRank_;

        //- Size of pre-flop prediction array = 21187660
        const static size_t preFlopPredictSize_;

        //- Size of flop prediction array = 1081
        const static size_t flopPredictSize_;

        //- Size of turn prediction array = 46
        const static size_t turnPredictSize_;



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

        //- Helper for compare
        //  Finds kickers, if any, given the provided hand type
        //  ** Located in HandRankerGetKickers.cpp
        static PktVals getKickers(
            const Board& bd,
            const PktCards& pkt,
            const HandType& ht
        );

        //- Return the exact ranking of the pocket cards among all possibilities
        //  *** Located in HandRankerRank.cpp
        short rank();


        // Predict functions based on river rank

            //- Return all possible riverRanks given an empty board.
            //  Array will be nCr(50,5) = 21187660 river rank values
            //  Does not sort result
            //  This function was used to create preflop rank histogram static data, and would be
            //  too slow to use live.
            //  *** Located in HandRankerPredict.cpp
            std::vector<short> predictPreFlopToRiver();

            //- Return all possible riverRanks after flop is showing
            //  Array will be nCr(47,2) = 1081 river rank values
            //  Does not sort result
            //  *** Located in HandRankerPredict.cpp
            std::vector<short> predictFlopToRiver();

            //- Return all possible riverRanks after flop is showing
            //  Array will be 46 river rank values
            //  Does not sort result
            //  *** Located in HandRankerPredict.cpp
            std::vector<short> predictTurnToRiver();

            //- Convert predict array into a histogram of values with spacing = 1
            //  *** Located in HandRankerPredict.cpp
            static std::vector<int> paToHistogram(std::vector<short> pa, short max=0);


        // Old predict functions

            //- Return array of rank values based on what the next card might be
            //  Suitable only after the flop and before the river.
            //  *** Located in HandRankerPredict.cpp
            void predictAndSort(std::vector<short>& pa);

            //- Return array of rank values based on what the flop might be
            //  Suitable only when the board is empty
            //  *** Located in HandRankerPredict.cpp
            void predictFlopAndSort(std::vector<short>& pa);


        // Win calculations

            //- Get river rank histogram for a given starting hand
            static const std::vector<int>& shrrHist(const PktCards& pkt);


            // Raw win probabilities (assumes all opponents could have any hand)
            // These are separated into preflop, flopTurn, and river because the data is handled
            // differently for each:
            //  * preflop - we use the preflop histogram static data
            //  * flop or turn - the application is expected to get its own predictionArray using
            //      predictFlopToRiver or predictTurnToRiver

                //- Calculate raw win probability from preflop, given hand and number of players
                static Scalar preflopWinProbRaw(const PktCards& pkt, size_t nOpponents);
                Scalar preflopWinProbRaw(size_t nOpponents);

                //- Calculate the raw win probability from flop or turn, given number of players
                static Scalar flopTurnWinProbRaw(
                    const std::vector<short>& riverRankHistogram,
                    size_t nOpponents
                );

                //- Calculate the raw win probability for a given river rank and players
                static Scalar riverWinProbRaw(short riverRank, size_t nOpponents);


            // Estimated win probabilities (provided estimates of opponent hand ranges)

                //- Calculate the estimated win probability, given flop to river predict array and
                //  estimates of rank range for all active opponents, given as:
                //  (lowerRank, higherRank)
                static Scalar preflopWinProbEst(
                    const PktCards& pkt,
                    const std::vector<std::pair<short, short>>& opponentRankEstimates
                );
                Scalar preflopWinProbEst(
                    const std::vector<std::pair<short, short>>& opponentRankEstimates
                );

                //- Calculate the estimated win probability for a given river rank, given the river
                // rank and estimates for the rank range for all active opponents, given as:
                //  (lowerRank, higherRank)
                static Scalar riverWinProbEst(
                    short riverRank,
                    const std::vector<std::pair<short, short>>& opponentRankEstimates
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
