#include <StraightCompleters.h>

// ****** Constructors ****** //

ds::StraightCompleters::StraightCompleters()
{}


ds::StraightCompleters::StraightCompleters(const VecCardVal& values) {
    makeStraightCompleters(values);
}


ds::StraightCompleters::StraightCompleters(const VecValStats& stats) {
    VecCardVal values;
    values.reserve(stats.size());
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        values.push_back(it->value());
    }
    makeStraightCompleters(values);
}


// ****** Public Member Functions ****** //

void ds::StraightCompleters::makeStraightCompleters (
    const VecCardVal& values
) {
    if (!values.size()) {
        return;
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
    if (values.front() == Card::ace) {
        cValues.push_back(Card::lowAce);
    }

    VecCardVal::const_iterator altVsIter(cValues.begin());
    if (altVsIter == cValues.end()) {
        return;
    }
    VecCardVal::const_iterator vsIter(altVsIter + 1);
    if (vsIter == cValues.end()) {
        return;
    }
    ++vsIter;
    while (vsIter != cValues.end() && (*altVsIter - *vsIter > 4)) {
        altVsIter++;
        vsIter++;
    }
    if (vsIter == cValues.end()) {
        return;
    }
    vsIter = altVsIter;
    reserve(5);
    short cursor = std::min(CardVal(*altVsIter + 2), Card::ace);
    short restartCursor = cursor;
    PktVals pocket(Card::wildValue, Card::wildValue);
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
        return;
    }
    while (1) {
        --cursor;
        ++curStraightSize;
        // check if next value is on board or there is room in pocket
        if (vsIter != cValues.end() && *vsIter == cursor) {
            // Value is on the board
            ++vsIter;
        } else if (pocket.first == Card::wildValue) {
            // There is room in the first pocket card
            pocket.first = cursor;
            
            // Set restart
            altVsIter = vsIter;
            restartCursor = cursor;
        } else if (pocket.second == Card::wildValue) {
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
            if (pocket.first == Card::lowAce) {
                pocket.first = Card::ace;
            } else if (pocket.second == Card::lowAce) {
                std::swap(pocket.second, pocket.first);
                pocket.first = Card::ace;
            }
            if (!size() || (size() && back().second != pocket)) {
                // Conditional accounts for connected pocket with a higher
                // straight value
                push_back(
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
}


// ****** Private Member Functions ****** //

bool ds::StraightCompleters::restart(
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
        pocket.second == Card::wildValue
     && pocket.first != Card::wildValue
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
        pocket.first = Card::wildValue;
    }
    pocket.second = Card::wildValue;
    return true;
}


// ****** END ****** //
