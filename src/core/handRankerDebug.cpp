#include<iterator>
#include<handRanker.h>
#include<error.h>

// *** Public Member Functions *** //

ds::straightCompleters ds::handRanker::findStraightCompleters (
    const valueSet& values
) {
    // &&& Forgot to account for A2345
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
    pocketValues pocket(0, 0);
    if (cursor == *vsIter) {
        ++vsIter;
    } else {
        pocket.first = cursor;
        restartCursor = cursor - 1;
        altVsIter = vsIter;
    }
    short curStraightSize(1);
    short curStraightMax(cursor);
short vsVal = *vsIter;
short altVsVal = *altVsIter;
std::cout << "    New state: (" << pocket.first << " " << pocket.second
    << ") cursors = " << cursor << " " << restartCursor
    << ", size/max = " << curStraightSize << " " << curStraightMax
    << ", iters = [" << vsVal << "] [" << altVsVal << "]" << std::endl;
    while (1) {
        // cursor points to up-to-date values
        // cursor is either in pocket or altVsIter
        // vsNextIter points to subsequent board card
        --cursor;
        ++curStraightSize;
        // check if next value is on board or there is room in pocket
        if (vsIter != values.rend() && *vsIter == cursor) {
            // Value is on the board
            ++vsIter;
std::cout << "Value " << cursor << " is on the board.  New iter = " << vsVal
    << ".\n";
vsVal = *vsIter;
altVsVal = *altVsIter;
std::cout << "    New state: (" << pocket.first << " " << pocket.second
    << ") cursors = " << cursor << " " << restartCursor
    << ", size/max = " << curStraightSize << " " << curStraightMax
    << ", iters = [" << vsVal << "] [" << altVsVal << "]" << std::endl;
        } else if (pocket.first == 0) {
            // There is room in the first pocket card
            pocket.first = cursor;
std::cout << "Adding " << cursor << " to pocket1.\n";
vsVal = *vsIter;
altVsVal = *altVsIter;
std::cout << "    New state: (" << pocket.first << " " << pocket.second
    << ") cursors = " << cursor << " " << restartCursor
    << ", size/max = " << curStraightSize << " " << curStraightMax
    << ", iters = [" << vsVal << "] [" << altVsVal << "]" << std::endl;
            setRestart(
                cursor,
                vsIter,
                altVsIter,
                restartCursor
            );
std::cout << "Setting restart.\n";
vsVal = *vsIter;
altVsVal = *altVsIter;
std::cout << "    New state: (" << pocket.first << " " << pocket.second
    << ") cursors = " << cursor << " " << restartCursor
    << ", size/max = " << curStraightSize << " " << curStraightMax
    << ", iters = [" << vsVal << "] [" << altVsVal << "]" << std::endl;
        } else if (pocket.second == 0) {
            // There is room in the second pocket card
            pocket.second = cursor;
std::cout << "Adding " << cursor << " to pocket2.\n";
vsVal = *vsIter;
altVsVal = *altVsIter;
std::cout << "    New state: (" << pocket.first << " " << pocket.second
    << ") cursors = " << cursor << " " << restartCursor
    << ", size/max = " << curStraightSize << " " << curStraightMax
    << ", iters = [" << vsVal << "] [" << altVsVal << "]" << std::endl;
        } else {
            // No straight formed
std::cout << "No straight was formed upon reaching " << cursor << ".\n";
vsVal = *vsIter;
altVsVal = *altVsIter;
std::cout << "    New state: (" << pocket.first << " " << pocket.second
    << ") cursors = " << cursor << " " << restartCursor
    << ", size/max = " << curStraightSize << " " << curStraightMax
    << ", iters = [" << vsVal << "] [" << altVsVal << "]" << std::endl;
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
std::cout << "Restarting." << cursor << ".\n";
vsVal = *vsIter;
altVsVal = *altVsIter;
std::cout << "    New state: (" << pocket.first << " " << pocket.second
    << ") cursors = " << cursor << " " << restartCursor
    << ", size/max = " << curStraightSize << " " << curStraightMax
    << ", iters = [" << vsVal << "] [" << altVsVal << "]" << std::endl;
            if (!restarted) {
std::cout << "Restart failed." << std::endl;
                break;
            }
std::cout << "Restart succeeded." << std::endl;
vsVal = *vsIter;
altVsVal = *altVsIter;
std::cout << "    New state: (" << pocket.first << " " << pocket.second
    << ") cursors = " << cursor << " " << restartCursor
    << ", size/max = " << curStraightSize << " " << curStraightMax
    << ", iters = [" << vsVal << "] [" << altVsVal << "]" << std::endl;
            continue;
        }
        if (curStraightSize == 5) {
            // Straight was formed
std::cout << "Straight was formed." << std::endl;
vsVal = *vsIter;
altVsVal = *altVsIter;
std::cout << "    New state: (" << pocket.first << " " << pocket.second
    << ") cursors = " << cursor << " " << restartCursor
    << ", size/max = " << curStraightSize << " " << curStraightMax
    << ", iters = [" << vsVal << "] [" << altVsVal << "]" << std::endl;
            sc.first.push_back(curStraightMax);
            sc.second.push_back(pocket);

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
std::cout << "Restarting." << cursor << ".\n";
vsVal = *vsIter;
altVsVal = *altVsIter;
std::cout << "    New state: (" << pocket.first << " " << pocket.second
    << ") cursors = " << cursor << " " << restartCursor
    << ", size/max = " << curStraightSize << " " << curStraightMax
    << ", iters = [" << vsVal << "] [" << altVsVal << "]" << std::endl;
            if (!restarted) {
std::cout << "Restart failed." << std::endl;
                break;
            }
std::cout << "Restart succeeded." << std::endl;
vsVal = *vsIter;
altVsVal = *altVsIter;
std::cout << "    New state: (" << pocket.first << " " << pocket.second
    << ") cursors = " << cursor << " " << restartCursor
    << ", size/max = " << curStraightSize << " " << curStraightMax
    << ", iters = [" << vsVal << "] [" << altVsVal << "]" << std::endl;
        }
    }

    return sc;
}

// *** Private Member Functions *** //

void ds::handRanker::setRestart (
    short cursor,
    const valueSet::const_reverse_iterator& vsIter,
    valueSet::const_reverse_iterator& altVsIter,
    short& restartCursor
) {
    altVsIter = vsIter;
    restartCursor = cursor;
}


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
        pocket.first = 0;
    }
    pocket.second = 0;
    if (std::distance(vsIter, values.rend()) < 2) {
        // No more possible straights
        return false;
    }
    return true;
}

