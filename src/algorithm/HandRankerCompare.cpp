#include<PktMask.h>

// ****** Public Member Functions ****** //

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
    
    // ht's are equal, check values.first
    if (htA.values.first > htB.values.first) {
        return 1;
    } else if (htA.values.first < htB.values.first) {
        return -1;
    }
    
    // values.first is equal, check values.second
    if (htA.values.second > htB.values.second) {
        return 1;
    } else if (htA.values.second < htB.values.second) {
        return -1;
    }

    // check first kicker
    if (htA.kickers.first > htB.kickers.first) {
        return 1;
    } else if (htA.kickers.first < htB.kickers.first) {
        return -1;
    }
    
    // check second kicker
    if (htA.kickers.second > htB.kickers.second) {
        return 1;
    } else if (htA.kickers.second < htB.kickers.second) {
        return -1;
    }
    
    return 0;
}

// ****** END ****** //
