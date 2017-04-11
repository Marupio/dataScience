#include<PktMask.h>

// ****** Public Member Functions ****** //

//- Compare two pockets against the board
//  Returns:
//  -1: pktA < pktB
//   0: pktA == pktB
//   1: pktA > pktB
short ds::HandRanker::compare
(
    const Board& bd,
    const PktCards& pktA,
    const PktCards& pktB
) {
    HandType htA(getHandType(bd, pktA));
    HandType htB(getHandType(bd, pktB));
    
    // check ht
    if (htA.ht > htB.ht) {
        return 1;
    } else if (htA.ht < htB.ht) {
        return -1;
    }
    
    // ht's are equal, check value.first
    if (htA.value.first > htB.value.first) {
        return 1;
    } else if (htA.value.first < htB.value.first) {
        return -1;
    }
    
    // value.first is equal, check value.second
    if (htA.value.second > htB.value.second) {
        return 1;
    } else if (htA.value.second < htB.value.second) {
        return -1;
    }

    switch (htA.ht) {
    case HtStraight: // fall through
    case HtFullHouse: // fall through
    case HtStraightFlush: {
        // No kickers possible
        return 0;
        break;
    }
    case HtHighCard: {
        // Two kickers
        break;
    }
    case HtPair: {
        // Two kickers
        break;
    }
    case HtTwoPair: {
        // One kicker
        break;
    }
    case HtSet: {
        // Two kickers
        break;
    }
    case HtFlush: {
        // Two suited kickers
        break;
    }
    case HtFoak: {
        // One kicker
        break;
    }
    default: {
        break;
    } // end default
    } // end switch

    // value.second is equal. Now we have to check for kickers.
    
    4 4 7 7 8
}

// ****** END ****** //
