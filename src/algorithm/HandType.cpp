#include<iterator>
#include<HandType.h>
#include<error.h>

// ****** Static Data Members ****** //

const char ds::HandType::HtUnknown       = 0;
const char ds::HandType::HtHighCard      = 1;
const char ds::HandType::HtPair          = 2;
const char ds::HandType::HtTwoPair       = 3;
const char ds::HandType::HtSet           = 4;
const char ds::HandType::HtStraight      = 5;
const char ds::HandType::HtFlush         = 6;
const char ds::HandType::HtFullHouse     = 7;
const char ds::HandType::HtFoak          = 8;
const char ds::HandType::HtStraightFlush = 9;

const std::array<std::string, 10> ds::HandType::HandTypeNames = {
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

ds::HandType::HandType(char t, PktVals v):
    ht(t),
    values(v)
{}


ds::HandType::HandType(char t, CardVal vA, CardVal vB):
    ht(t),
    values(vA, vB)
{}


// ****** END ****** //
