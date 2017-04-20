// ****** Private Member Functions ****** //

ds::PktVals ds::HandRanker::getKickers(
    const Board& bd,
    const PktCards& pkt,
    const HandType& ht
) {
    PktVals kickers(PktVals::unknownValues);
    switch (ht.ht()) {
    case HandType::HtStraightFlush: // fall through
    case HandType::HtFullHouse: // fall through
    case HandType::HtStraight: {
        // No kickers
        return kickers;
    }
    case HandType::HtFoak: {
        const VecValStats& stats(bd.stats());
        auto it = stats.cbegin();
        while (it != stats.cend() && it->value() != ht.values().first) {
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
              ? kickers.first
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
                  ? kickers.first
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
    case HandType::HtFlush: {
        const VecCardVal& flushValues(bd.flushValues());
        const Suit flushSuit(bd.flushSuit());
        switch (flushValues.size()) {
        case 5: {
            const PktVals lowVals(bd.lowestTwoFlushValues());
            if (pkt.first.suit() == flushSuit) {
                kickers.first = pkt.first.value();
            }
            if (pkt.second.suit() == flushSuit) {
                kickers.second = pkt.second.value();
            }
            kickers.sort();

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
        case 4: {
            const CardVal lowVal(bd.lowestFlushValue());
            if (pkt.first.suit() == flushSuit) {
                kickers.first = pkt.first.value();
            }
            if (pkt.second.suit() == flushSuit) {
                kickers.second = pkt.second.value();
            }
            kickers.sort();

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
            FatalError << "Flush is expected, and flushValues has incorrect "
                << "size (" << flushValues.size() << "). Must be 3,4 or 5."
                << std::endl;
            abort();
        } // end default
        } // end switch
        FatalError << "Internal error. Kickers should have been returned."
            << std::endl;
        abort();
        break;
    }
    case HandType::HtSet: {
        const VecValStats& stats(bd.stats());
        auto it = stats.cbegin();
        while (it != stats.cend() && it->value() != ht.values().first) {
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
            if (kickers.first < highVals.second) {
                kickers = PktVals::unknownValues;
            } else if (
                kickers.first < highVals.first
             || kickers.second < highVals.second
            ) {
                kickers.second = Card::unknownValue;
            }
            return kickers;
        }
        case 2: {
            // Set uses a pair on the board, one pocket card is free

            kickers.first = pkt.highestValue(it->value());

            // Get second lowest board value, not including set value
            CardVal lowVal = bd.lowestTwoValues(it->value()).second;
            
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
    case HandType::HtTwoPair: {
        const VecValStats& stats(bd.stats());
        auto itA = stats.cbegin();
        while (itA != stats.cend() && itA->value() != ht.values().first) {
            ++itA;
        }
        auto itB = stats.cbegin();
        while (itB != stats.cend() && itB->value() != ht.values().second) {
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
            CardVal lowVal = bd.highestValue(ht.values());
            if (kickers.first <= lowVal) {
                kickers.first = Card::unknownValue;
            }
            return kickers;
        }
        // Either A or B are paired on the board:
        //  * One pocket pairs with board
        //  * Two board cards challenge kicker
        //  BBB?? P?
        if (
            pkt.first.value() == itA->value()
         || pkt.first.value() == itB->value()
        ) {
            kickers.first = pkt.second.value();
        } else {
            kickers.first = pkt.first.value();
        }

        const CardVal lowVal(bd.highestValue(ht.values()));
        if (kickers.first <= lowVal) {
            kickers.first = Card::unknownValue;
        }
        return kickers;
    }
    case HandType::HtPair: {
        if (pkt.pairs()) {
            return kickers;
        }
        const VecValStats& stats(bd.stats());
        auto it = stats.cbegin();
        while (it != stats.cend() && it->value() != ht.values().first) {
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
            // XX??? ??

            kickers = pkt.sortedValues();
            const PktVals lowVals(bd.lowestTwoValues(ht.values().first));
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

        if (pkt.first.value() == ht.values().first) {
            kickers.first = pkt.second.value();
        } else {
            kickers.first = pkt.first.value();
        }

        const CardVal lowVal(bd.lowestValue(ht.values().first));
        if (kickers.first <= lowVal) {
            kickers.first = Card::unknownValue;
        }
        return kickers;
    }
    case HandType::HtHighCard: {
        kickers = pkt.sortedValues();
        const PktVals lowVals(bd.lowestTwoValues(ht.values().first));
        
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
        FatalError << "Unknown HandType (" << ht.ht() << ")" << std::endl;
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
