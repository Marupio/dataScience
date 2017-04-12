#include<PktMask.h>

// ****** Public Member Functions ****** //

short ds::HandRanker::compare
(
    const Board& bd,
    const PktCards& pktA,
    const PktCards& pktB
) {
    HandTypeStruct htA(getHandType(bd, pktA));
    HandTypeStruct htB(getHandType(bd, pktB));
    
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
        return findAndCompareKickers
        (
            bd,
            PktVals(Card::unknownValue, Card::unknownValue),
            Card::unknownSuit,
            true,
            pktA,
            pktB
        );

        break;
    }
    case HtSet: // fall through
    case HtPair: {
        // Two kickers
        return findAndCompareKickers
        (
            bd,
            PktVals(htA.values.first, Card::unknownValue),
            Card::unknownSuit,
            true,
            pktA,
            pktB
        );
        break;
    }
    case HtTwoPair: {
        // One kicker
        return findAndCompareKickers
        (
            bd,
            PktVals(htA.values.first, htA.values.second),
            Card::unknownSuit,
            false,
            pktA,
            pktB
        );
        break;
    }
    case HtFlush: {
        // Two suited kickers
        return findAndCompareKickers
        (
            bd,
            PktVals(Card::unknownValue, Card::unknownValue),
            bd.flushSuit(),
            true,
            pktA,
            pktB
        );
        break;
    }
    case HtFoak: {
        // One kicker
        return findAndCompareKickers
        (
            bd,
            PktVals(htA.values.first, htA.values.second),
            Card::unknownSuit,
            false,
            pktA,
            pktB
        );
        break;
    }
    default: {
        FatalError << "Unknown hand types: " << bd << " " << pktA << " "
            << pktB << std::endl;
            abort();
        return -2;
        break;
    } // end default
    } // end switch
}


// ****** Private Member Functions ****** //

short ds::HandRanker::findAndCompareKickers
(
    const Board& bd,
    const PktVals& avoid,
    Suit requiredSuit,
    bool checkSecond,
    const PktCards& pktA,
    const PktCards& pktB
) {
    const VecValStats& stats(bd.stats());
    auto itR = stats.crbegin();
    CardVal bdLowKicker = Card::lowAce;
    CardVal bdHighKicker = Card::lowAce;
    while (
        itR != stats.crend()
     && (
            bdLowKicker == Card::lowAce
         || bdLowKicker == avoid.first
         || bdLowKicker == avoid.second
        )
    ) {
        bdLowKicker = itR->value();
        ++itR;
    }
    while (
        itR != stats.crend()
     && (
            bdHighKicker == Card::lowAce
         || bdHighKicker == avoid.first
         || bdHighKicker == avoid.second
        )
    ) {
        bdHighKicker = itR->value();
        ++itR;
    }
std::cout << "bdKickers = (" << bdLowKicker << " " << bdHighKicker << ") ";
    PktVals kickersA = findTwoKickers(
        pktA,
        avoid,
        bdLowKicker,
        bdHighKicker,
        requiredSuit
    );
std::cout << "kickersA = " << kickersA << " ";

    PktVals kickersB = findTwoKickers(
        pktB,
        avoid,
        bdLowKicker,
        bdHighKicker,
        requiredSuit
    );
std::cout << "kickersB = " << kickersB << std::endl;

    return checkKickers(kickersA, kickersB, checkSecond);
}


ds::PktVals ds::HandRanker::findTwoKickers(
    const PktCards& pkt,
    const PktVals& avoid,
    CardVal bdLowKicker,
    CardVal bdHighKicker,
    Suit requiredSuit
) {
    CardVal pktHigh = Card::lowAce;
    CardVal pktLow = Card::lowAce;
    if (requiredSuit == Card::unknownSuit || pkt.suited(requiredSuit)) {
        if (pkt.first.value() > pkt.second.value()) {
            pktHigh = pkt.first.value();
            pktLow = pkt.second.value();
        } else {
            pktHigh = pkt.second.value();
            pktLow = pkt.first.value();
        }
    } else if (pkt.first.suit() == requiredSuit) {
        pktHigh = pkt.first.value();
    } else if (pkt.second.suit() == requiredSuit) {
        pktHigh = pkt.second.value();
    }
    short avoidHits = 0;
    if (pktHigh == avoid.first || pktHigh == avoid.second) {
        if (pktLow == avoid.first || pktLow == avoid.second) {
            
        }
    }

    PktVals kickers;
    kickers.first = std::max(pktHigh, bdHighKicker);
    if (kickers.first == pktHigh) {
        kickers.second = std::max(bdHighKicker, pktLow);
    } else {
        kickers.second = std::max(bdLowKicker, pktHigh);
    }

    short avoidHits = 0;
    if (pktHigh == avoid.first || pktHigh == avoid.second) {
        avoidHits++;
        kickers.second = Card::lowAce;
    }
    if (pktLow == avoid.first || pktLow == avoid.second) {
        avoidHits++;
        kickers.second = Card::lowAce;
    }
    if (avoidHits > 1){
        kickers.first = Card::lowAce;
    }
    return kickers;
}


short ds::HandRanker::checkKickers
(
    const PktVals& kickersA,
    const PktVals& kickersB,
    bool checkSecond
) {
    // Check first kicker
    if (kickersA.first > kickersB.first) {
        return 1;
    } else if (kickersA.first < kickersB.first) {
        return -1;
    }
    
    if (checkSecond) {
        // Check second kicker
        if (kickersA.second > kickersB.second) {
            return 1;
        } else if (kickersA.second < kickersB.second) {
            return -1;
        }
    }
    
    // Kickers are the same.  Split.
    return 0;
}

// ****** END ****** //
