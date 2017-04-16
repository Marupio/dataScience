// ****** Public Member Functions ****** //

short ds::HandRanker::compare2
(
    const Board& bd,
    const PktCards& pktA,
    const PktCards& pktB
) {
    HandType2 htA(getHandType2(bd, pktA));
    HandType2 htB(getHandType2(bd, pktB));
    
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

    // All is equal, we need to check kickers
    PktVals kA = getKickers(bd, pktA, htA);
    PktVals kB = getKickers(bd, pktB, htB);

    // Check first kicker
    if (kA.first > kB.first) {
        return 1;
    } else if (kA.first < kB.first) {
        return -1;
    }

    // Check second kicker
    if (kA.second > kB.second) {
        return 1;
    } else if (kA.second < kB.second) {
        return -1;
    }

    return 0;
}


// ****** END ****** //
