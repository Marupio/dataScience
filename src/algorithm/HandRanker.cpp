#include <iterator>
#include <HandRanker.h>
#include <error.h>

// ****** Constructors ****** //

ds::HandRanker::HandRanker(
    const Board& bd,
    const PktCards& pkt,
    bool includePktInMask
):
    pkt_(pkt),
    bd_(bd),
    cbd_(bd),
    baseMask_(
        includePktInMask
      ? PktMask(bd, pkt)
      : PktMask(bd)
    ),
    mask_(baseMask_)
{}


#include <HandRankerGetKickers.cpp>
#include <HandRankerCompare.cpp>
#include <HandRankerPredict.cpp>
#include <HandRankerRank.cpp>
#include <HandRankerRankKickers.cpp>

// ****** END ****** //
