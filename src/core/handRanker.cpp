#include<iterator>
#include<handRanker.h>
#include<error.h>

// *** Public Member Functions *** //

ds::straightCompleters ds::handRanker::findStraightCompleters (
    const valueSet& values
) {
    // &&& Optimize by looking at iter and iter+2 for diff of 5
    bool aceOnBoard = values.back() == 14;
    straightCompleters sc;
    valueSet::const_reverse_iterator altVsIter(values.rbegin());
    if (altVsIter == values.rend()) {
        return sc;
    }
    valueSet::const_reverse_iterator vsIter(altVsIter + 1);
    while (vsIter != values.rend() && (*altVsIter - *vsIter > 3)) {
        altVsIter++;
        vsIter++;
    }
    vsIter = altVsIter;
    if (vsIter == values.rend()) {
        return sc;
    }
    sc.first.reserve(5);
    sc.second.reserve(5);

    short cursor = std::min((*altVsIter + 2), 14);
    short restartCursor = 0;
    altVsIter = values.rend();
    pocketValues pocket(15, 15);
    if (cursor == *vsIter) {
        ++vsIter;
    } else {
        pocket.first = cursor;
        restartCursor = cursor - 1;
        altVsIter = vsIter;
    }
    short curStraightSize(1);
    short curStraightMax(cursor);
    while (1) {
        // cursor points to up-to-date values
        // cursor is either in pocket or altVsIter
        // vsNextIter points to subsequent board card
        --cursor;
        if (cursor == 0) {
            // No more straights
            break;
        }
        ++curStraightSize;
        // check if next value is on board or there is room in pocket
        if (vsIter != values.rend() && *vsIter == cursor) {
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
        } else if (!(cursor == 1 && aceOnBoard)) {
            // Above conditional accounts for Ace on board acting as low
            // No straight formed
            bool restarted = restart(
                values,
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
            continue;
        }
        if (curStraightSize == 5) {
            // Straight was formed
            // Account for low Ace value
            if (pocket.first == 1) {
                if (aceOnBoard) {
                    pocket.first = 15;
                } else {
                    pocket.first = 14;
                }
            } else if (pocket.second == 1) {
                if (aceOnBoard) {
                    pocket.second = 15;
                } else {
                    std::swap(pocket.second, pocket.first);
                    pocket.first = 14;
                }
            }
            if (sc.second.back() != pocket) {
                // Conditional accounts for connected pocket with a higher
                // straight value
                sc.first.push_back(curStraightMax);
                sc.second.push_back(pocket);
            }

            bool restarted = restart(
                values,
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
    if (std::distance(vsIter, values.rend()) < 2) {
        // No more possible straights
        return false;
    }
    return true;
}

