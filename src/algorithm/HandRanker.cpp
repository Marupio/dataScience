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

ds::HandRanker::HandType::HandType(char t, PktVals v):
    ht(t),
    values(v)
{}


ds::HandRanker::HandType::HandType(char t, CardVal vA, CardVal vB):
    ht(t),
    values(vA, vB)
{}


#include<HandRankerGetHandType.cpp>
#include<HandRankerGetKickers.cpp>
#include<HandRankerCompare.cpp>
#include<HandRankerRank.cpp>

// ****** END ****** //
