#ifndef handRanker_h
#define handRanker_h

#include<straightCompleters.h>

namespace ds {

class handRanker
{

public:

    //- Find pocket cards that complete a straight for a given set of values
    //  Values must be sorted and unique, it doesn't check
    static straightCompleters findStraightCompleters(const valueSet& values);

private:

    //- Helper function for findStraightCompleters
    static bool restart(
        const valueSet& values,
        short& cursor,
        short& restartCursor,
        valueSet::const_reverse_iterator& vsIter,
        valueSet::const_reverse_iterator& altVsIter,
        short& curStraightSize,
        short& curStraightMax,
        pocketValues& pocket
    );

};

}
#endif
