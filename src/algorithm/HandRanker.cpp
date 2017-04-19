#include<iterator>
#include<HandRanker.h>
#include<error.h>

// ****** Constructors ****** //

ds::HandRanker::HandRanker(const Board& bd, const PktCards& pkt):
    bd_(bd),
    pkt_(pkt),
    baseMask_(bd, pkt),
    mask_(baseMask_)
{}


#include<HandRankerGetKickers.cpp>
#include<HandRankerCompare.cpp>
#include<HandRankerRank.cpp>

// ****** END ****** //
