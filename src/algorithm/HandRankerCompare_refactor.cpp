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


ds::PktVals getKickers(
    const Board& bd,
    const PktCards& pkt,
    const HandType& ht
) {
    PktVals kickers(PktVals::unknownValues);
    switch (ht.ht) {
    case HtStraightFlush: // fall through
    case HtFullHouse: // fall through
    case HtStraight: {
        // No kickers
        break;
    }
    case HtFoak: {
        const VecValStats& stats(bd.stats());
        auto it = stats.cbegin();
        while (it != stats.cend() && it->value() != ht.values.first) {
            ++it;
        }
        #ifdef DSDEBUG
        if (it == stats.cend()) {
            FatalError << "Could not find FOAK value reported by HandType."
                << std::endl;
            abort();
        }
        #endif
        switch (it->nCards()) {
        case 4: {
            kickers.first = pkt.highestValue();
            CardVal highVal = bd.highestValue(it->value());
            kickers.first =
                kickers.first > highVal
              ? kicker
              : Card::unknownValue;
            return kickers;
        }
        case 3: {
            // Set is on the board
            if (pkt.has(it->value()))
            {
                kickers.first = pkt.highestValue(it->value());
                CardVal highVal = bd.highestValue(it->value());
                kickers.first =
                    kickers.first > highVal
                  ? kicker
                  : Card::unknownValue;
                return kickers;
            }
            break;
        }
        case 2: {
            // Pair is on the board
            if (pkt.pairs(it->value())) {
                return kickers;
            }
            break;
        }
        default: {
            FatalError << "Could not find FOAK reported by HandType"
                << std::endl;
            abort();
            break;
        } // end default
        } // end switch
        FatalError << "Could not find FOAK reported by HandType" << std::endl;
        abort();
        break;
    }
    case HtFlush: {
        const VecCardVal& flushVals(bd.flushValues());
        switch (flushVals.size()) {
        case 5: {
            const PktVals lowVals(bd.lowestTwoFlushValues());
            kickers = pkt.sortedValues();
            // Check two kickers
            if (kickers.first < lowValA) {
                kickers = PktVals::unknownValues;
            } else if (kickers.first < lowValB || kickers.second < lowValA) {
                kickers.second = Card::unknownValue;
            }
            return kickers;
        }
        case 4: {
            const CardVal lowVal(bd.lowestFlushValue());
            kickers = pkt.sortedValues();
            if (kickers.second < lowVal) {
                kickers.second = Card::unknownValue;
            }
            return kickers;
        }
        case 3: {
            kickers = pkt.sortedValues();
            return kickers;
        }
        default: {
            FatalError << "Flush is expected, and flushVals has incorrect size "
                << "(" << flushVals.size() << "). Must be 3,4 or 5."
                << std::endl;
            abort();
        } // end default
        } // end switch
        FatalError << "Internal error. Kickers should have been returned."
            << std::endl;
        abort();
        break;
    }
    case HtSet: {
        const VecValStats& stats(bd.stats());
        auto it = stats.cbegin();
        while (it != stats.cend() && it->value() != ht.values.first) {
            ++it;
        }
        #ifdef DSDEBUG
        if (it == stats.cend()) {
            FatalError << "Could not find set value reported by HandType."
                << std::endl;
            abort();
        }
        #endif
        switch (it->nCards()) {
        case 3: {
            // Set is on board, look for two kickers
            
            const PktVals highVals(bd.highestTwoValues(it->value()));
            kickers = pkt.sortedValues();
            
            // Check two kickers            
            if (kickers.first < highVals.first) {
                kickers = noKickers;
            } else if (
                kickers.first < highVals.second
             || kickers.second < highVals.first
            ) {
                kickers.second = Card::unknownValue;
            }
            return kickers;
        }
        case 2: {
            // Set uses a pair on the board, one pocket card is free

            kickers.first = pkt.highestValue(it->value());

            // Get second lowest board value, not including set value
            CardVal lowVal = bd.findLowestTwoVals(it->value()).second();
            
            if (kickers.first <= lowVal) {
                kickers.first = Card::unknownValue;
            }
            return kickers;
        }
        case 1: {
            return kickers;
        }
        default: {
            FatalError << "Unexpected nCards when checking for Set. nCards "
                << "is: " << it->nCards() << std::endl;
            abort();
        } // end default
        } // end switch
        break;
    }
    case HtTwoPair: {
        const VecValStats& stats(bd.stats());
        auto itA = stats.cbegin();
        while (itA != stats.cbegin() && itA->value() != ht.values.first) {
            ++itA;
        }
        auto itB = stats.cbegin();
        while (itB != stats.cbegin() && itB->value() != ht.values.first) {
            ++itB;
        }
        #ifdef DSDEBUG
        if (itA == stats.cend() && itB == stats.cend()) {
            FatalError << "Could not find two pair reported by HandType."
                << std::endl;
            abort();
        }
        #endif
        if (
            itA == stats.cend()
         || itB == stats.cend()
         || (itA->nCards() == 1 && itB->nCards() == 1)
        ) {
            // Pocket pairs used or both pocket cards used to pair with board
            // No kickers
            return kickers;
        }
        if (itA->nCards() == 2 && itB->nCards() == 2) {
            // Both pairs are on board, pocket is free
            kickers.first = pkt.highestValue();
            CardVal lowVal = bd.lowestValue(ht.values);
            if (kickers.first <= lowVal) {
                kickers.first = Card::unknownValue;
            }
            return kickers;
        }
        // Either A or B are paired on the board:
        //  * One pocket pairs with board
        //  * Two board cards challenge kicker
        if (
            pkt.first.value() == bd.pairA() || pkt.first.value() == bd.pairB()
        ) {
            kickers.first = pkt.second.value();
        } else {
            kickers.first = pkt.first.value();
        }
        
        const PktVals lowVals(bd.lowestTwoValues(ht.values));
        if (kickers.first <= ht.values.second) {
            kickers.first = Card::unknownCard;
        }
        return kickers;
    }
    case HtPair: {
        if (pkt.pairs()) {
            return kickers;
        }
        const VecValStats& stats(bd.stats());
        auto it = stats.cbegin();
        while (it != stats.cend() && it->value() != ht.values.first) {
            ++it;
        }
        #ifdef DSDEBUG
        if (it == stats.cend()) {
            FatalError << "Could not find pair that HandType reported."
                << std::endl;
            abort();
        }
        #endif
        if (it->nCards() == 2) {
            // Pair is on the board, pocket is free

            kickers = pkt.sortedValues();
            const PktVals lowVals(bd.lowestTwoValues(ht.values.first));

            // Check two kickers
            if (kickers.first < lowVals.first) {
                kickers = PktVals::unknownValues;
            } else if (
                kickers.first < lowVals.second
             || kickers.second < lowVals.first
            ) {
                kickers.second = Card::unknownValue;
            }
            return kickers;
        }
        // Paired with single, one pocket card is free
        // X???? X?

        if (pkt.first.value() == ht.values.first) {
            kickers.first = pkt.second.value();
        } else {
            kickers.first = pkt.first.value();
        }

        const CardVal lowVal(bd.lowestValue(ht.values.first));
        if (kickers.first <= lowVal) {
            kickers.first = Card::unknownValue;
        }
        return kickers;
    }
    case HtHighCard: {
        kickers = pkt.sortedValues();
        const PktVals lowVals(bd.lowestTwoValues(ht.values.first));
        
        // Check two kickers
        if (kickers.first < lowVals.first) {
            kickers = PktVals::unknownValues;
        } else if (
            kickers.first < lowVals.second
         || kickers.second < lowVals.first
        ) {
            kickers.second = Card::unknownValue;
        }
        return kickers;
    }
    default: {
        FatalError << "Unknown HandType (" << ht.ht << ")" << std::endl;
        abort();
        break;
    } // end default
    } // end switch
    FatalError << "Could not find kickers." << std::endl;
    abort();
    
    // Suppress warnings
    return kickers;
}

// ****** END ****** //
