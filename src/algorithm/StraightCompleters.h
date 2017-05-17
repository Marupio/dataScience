#ifndef StraightCompleters_h
#define StraightCompleters_h

#include <types.h>
#include <vector.h>
#include <VecValStats.h>

namespace ds {

class StraightCompleters:
    public std::vector<std::pair<CardVal, PktVals>>
{

public:

    // Constructors

        //- Construct null
        StraightCompleters();
        
        //- Construct given VecCardVal
        StraightCompleters(const VecCardVal& values);

        //- Construct given VecValStats
        StraightCompleters(const VecValStats& stats);


private:

    // Private member functions

        //- Find pocket cards that complete a straight for a given set of values
        //  Values must be sorted and unique, it doesn't check
        void makeStraightCompleters
        (
            const VecCardVal& values
        );

        //- Helper function for makeStraightCompleters
        bool restart(
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
