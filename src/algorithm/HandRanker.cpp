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


// ****** Public Member Functions ****** //

ds::StraightCompleters ds::HandRanker::findStraightCompleters (
    const VecCardVal& values
) {
    StraightCompleters sc;
    if (!values.size()) {
        return sc;
    }
    #ifdef DSDEBUG
    if (values.size() > 1) {
        if (values.front() < values.back()) {
            FatalError << "Values are not sorted highest to lowest.  Values "
                << "are:\n" << values << std::endl;
            abort();
        }
    }
    #endif
    VecCardVal cValues(values);
    if (values.front() == 14) {
        cValues.push_back(1);
    }

    VecCardVal::const_iterator altVsIter(cValues.begin());
    if (altVsIter == cValues.end()) {
        return sc;
    }
    VecCardVal::const_iterator vsIter(altVsIter + 1);
    if (vsIter == cValues.end()) {
        return sc;
    }
    ++vsIter;
    while (vsIter != cValues.end() && (*altVsIter - *vsIter > 4)) {
        altVsIter++;
        vsIter++;
    }
    if (vsIter == cValues.end()) {
        return sc;
    }
    vsIter = altVsIter;
    sc.reserve(5);
    short cursor = std::min((*altVsIter + 2), 14);
    short restartCursor = cursor;
    PktVals pocket(15, 15);
    short curStraightSize(1);
    CardVal curStraightMax(cursor);

    bool started = restart(
        cValues,
        cursor,
        restartCursor,
        vsIter,
        altVsIter,
        curStraightSize,
        curStraightMax,
        pocket
    );
    if (!started) {
        return sc;
    }
    while (1) {
        --cursor;
        ++curStraightSize;
        // check if next value is on board or there is room in pocket
        if (vsIter != cValues.end() && *vsIter == cursor) {
            // Value is on the board
            ++vsIter;
        } else if (pocket.first == 15) {
            // There is room in the first pocket card
            pocket.first = cursor;
            
            // Set restart
            altVsIter = vsIter;
            restartCursor = cursor;
        } else if (pocket.second == 15) {
            // There is room in the second pocket card
            pocket.second = cursor;
        }
        #ifdef DSDEBUG
        else {
            FatalError << "Failed to form a straight, error with input "
                << "values. Must be sorted and contain no duplicates. Values "
                << "are: " << std::endl;
            for (
                VecCardVal::const_iterator it = values.begin();
                it != values.end();
                ++it
            ) {
                std::cerr << *it << " ";
            }
            std::cerr << std::endl;
            abort();
        }
        #endif
        if (curStraightSize == 5) {
            // Straight was formed
            // Account for low Ace value
            if (pocket.first == 1) {
                pocket.first = 14;
            } else if (pocket.second == 1) {
                std::swap(pocket.second, pocket.first);
                pocket.first = 14;
            }
            if (!sc.size() || (sc.size() && sc.back().second != pocket)) {
                // Conditional accounts for connected pocket with a higher
                // straight value
                sc.push_back(
                    std::pair<CardVal, PktVals>(curStraightMax, pocket)
                );
            }

            bool restarted = restart(
                cValues,
                cursor,
                restartCursor,
                vsIter,
                altVsIter,
                curStraightSize,
                curStraightMax,
                pocket
            );
            if (!restarted) {
                break;
            }
        }
    }

    return sc;
}

// ****** Private Member Functions ****** //

bool ds::HandRanker::restart(
    const VecCardVal& values,
    short& cursor,
    short& restartCursor,
    VecCardVal::const_iterator& vsIter,
    VecCardVal::const_iterator& altVsIter,
    short& curStraightSize,
    CardVal& curStraightMax,
    PktVals& pocket
) {
    if (altVsIter == values.end()) {
        return false;
    }
    cursor = restartCursor;
    // Edge case - one wild card
    if (
        pocket.second == 15
     && pocket.first != 15
     && pocket.first < curStraightMax) {
        --cursor;
    }
    // There is another edge case with connected pockets and a higher
    // curMaxStraight where we'd have to do: cursor = cursor - 2
    // but it is faster to check for duplicate pocket cards when recording
    // a straight
    vsIter = altVsIter;
    if (vsIter != values.begin() && *(vsIter - 1) - cursor == 1) {
        --cursor;
    }
    // Increment altVsIter to two ahead
    ++altVsIter;
    if (altVsIter == values.end()) {
        return false;
    }
    ++altVsIter;
    while (1) {
        // Search for next good start point

        // Are there enough board cards and is the cursor high enough?
        if (altVsIter == values.end() || cursor < 5) {
            // No more straights
            return false;
        }

        // Is the value difference acceptable?
        if (cursor - *altVsIter < 5) {
            // Value difference is acceptable, proceed
            break;
        }

        // Move on
        if (cursor == *vsIter) {
            ++vsIter;
            ++altVsIter;
        }
        --cursor;
    }
    altVsIter = values.end();
    curStraightSize = 1;
    curStraightMax = cursor;
    if (*vsIter != cursor) {
        pocket.first = cursor;
        altVsIter = vsIter;
        restartCursor = cursor - 1;
    } else {
        altVsIter = values.end();
        vsIter++;
        pocket.first = 15;
    }
    pocket.second = 15;
    return true;
}

#include<HandRankerGetHandType.cpp>
#include<HandRankerCompare.cpp>
#include<HandRankerRank.cpp>

// ****** END ****** //
