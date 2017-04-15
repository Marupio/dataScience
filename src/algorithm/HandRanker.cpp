#include<iterator>
#include<HandRanker.h>
#include<error.h>

// ****** Static Data Members ****** //

const char ds::HandRanker::HtUnknown       = 0;
const char ds::HandRanker::HtHighCard      = 1;
const char ds::HandRanker::HtPair          = 2;
const char ds::HandRanker::HtTwoPair       = 3;
const char ds::HandRanker::HtSet           = 4;
const char ds::HandRanker::HtStraight      = 5;
const char ds::HandRanker::HtFlush         = 6;
const char ds::HandRanker::HtFullHouse     = 7;
const char ds::HandRanker::HtFoak          = 8;
const char ds::HandRanker::HtStraightFlush = 9;

const std::array<std::string, 10> ds::HandRanker::HandTypeNames = {
    "Unknown",
    "HighCard",
    "Pair",
    "TwoPair",
    "Set",
    "Straight",
    "Flush",
    "FullHouse",
    "Four-of-a-kind",
    "StraightFlush"
};

// ****** Constructors ****** //

ds::HandRanker::HandType::HandType(char t, PktVals v, PktVals k):
    ht(t),
    values(v),
    kickers(k)
{}


ds::HandRanker::HandType::HandType(char t, CardVal vA, CardVal vB, PktVals k):
    ht(t),
    values(vA, vB),
    kickers(k)
{}


ds::HandRanker::HandType::HandType(char t, PktVals v, CardVal kA, CardVal kB):
    ht(t),
    values(v),
    kickers(kA, kB)
{}


ds::HandRanker::HandType::HandType(
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


#include<HandRankerGetHandType.cpp>
#include<HandRankerCompare.cpp>
#include<HandRankerRank.cpp>

// ****** END ****** //
