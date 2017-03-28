#include<iterator>
#include<handRanker.h>
#include<error.h>

// *** Public Member Functions *** //

ds::straightCompleters ds::handRanker::findStraightCompleters (
    const valueSet& values
) {
    // &&& Optimize by looking at iter and iter+2 for diff of 5
    bool aceOnBoard = values.back() == 14;
    valueSet cValues;
    if (aceOnBoard) {
        cValues.reserve(values.size() + 1);
        cValues.push_back(1);
    } else {
        cValues.reserve(values.size());
    }
    cValues.insert(cValues.end(), values.begin(), values.end());

    straightCompleters sc;
    valueSet::const_reverse_iterator altVsIter(cValues.rbegin());
    if (altVsIter == cValues.rend()) {
        return sc;
    }
    valueSet::const_reverse_iterator vsIter(altVsIter + 1);
    if (vsIter == cValues.rend()) {
        return sc;
    }
    ++vsIter;
    while (vsIter != cValues.rend() && (*altVsIter - *vsIter > 4)) {
        altVsIter++;
        vsIter++;
    }
    if (vsIter == cValues.rend()) {
        return sc;
    }
    vsIter = altVsIter;
    sc.first.reserve(5);
    sc.second.reserve(5);
    short cursor = std::min((*altVsIter + 2), 14);
    short restartCursor = cursor;
    pocketValues pocket(15, 15);
    short curStraightSize(1);
    short curStraightMax(cursor);

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
        if (vsIter != cValues.rend() && *vsIter == cursor) {
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
                valueSet::const_iterator it = values.begin();
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
            if (sc.second.back() != pocket) {
                // Conditional accounts for connected pocket with a higher
                // straight value
                sc.first.push_back(curStraightMax);
                sc.second.push_back(pocket);
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

// *** Private Member Functions *** //

bool ds::handRanker::restart(
    const valueSet& values,
    short& cursor,
    short& restartCursor,
    valueSet::const_reverse_iterator& vsIter,
    valueSet::const_reverse_iterator& altVsIter,
    short& curStraightSize,
    short& curStraightMax,
    pocketValues& pocket
) {
    if (altVsIter == values.rend()) {
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
    if (vsIter != values.rbegin() && *(vsIter - 1) - cursor == 1) {
        --cursor;
    }
    // Increment altVsIter to two ahead
    ++altVsIter;
    if (altVsIter == values.rend()) {
        return false;
    }
    ++altVsIter;
    while (1) {
        // Search for next good start point

        // Are there enough board cards and is the cursor high enough?
        if (altVsIter == values.rend() || cursor < 5) {
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
    altVsIter = values.rend();
    curStraightSize = 1;
    curStraightMax = cursor;
    if (*vsIter != cursor) {
        pocket.first = cursor;
        altVsIter = vsIter;
        restartCursor = cursor - 1;
    } else {
        altVsIter = values.rend();
        vsIter++;
        pocket.first = 15;
    }
    pocket.second = 15;
    return true;
}

