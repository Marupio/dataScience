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
            kickers.first = pkt.highestVal();
            CardVal highVal = bd.highestVal(it->value());
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
                kickers.first = pkt.highestVal(it->value());
                CardVal highVal = bd.highestVal(it->value());
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
        const VecCardVal& flushVals(bd.flushVals());
        switch (flushVals.size()) {
        case 5: {
            const PktVals lowVals(bd.lowestTwoFlushVals());
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
            const CardVal lowVal(bd.lowestFlushVal());
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
                << " (" << flushVals.size() << "). Must be 3,4 or 5."
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
            
            const PktVals highVals(bd.highestTwoVals(it->value()));
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

            kickers.first = pkt.highestVal(it->value());

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

sizes
a b k
2 2 2
2 1 1
1 1 0

        break;
    }
    case HtPair: {
        break;
    }
    case HtHighCard: {
        break;
    }
    default: {
        FatalError << "Unknown HandType (" << ht.ht << ")" << std::endl;
        abort();
        break;
    } // end default
    } // end switch






    // Check for two pair
    if (bd.pairA() != Card::unknownValue) {
        if (pkt.pairs()) {
            if (pkt.first.value() > bd.pairA()) {
                return HandType(
                    HtTwoPair,
                    pkt.first.value(),
                    bd.pairA(),
                    noKickers
                );
            }
            if (bd.pairB() != Card::unknownValue)
            {
                CardVal kicker;
                CardVal pairB;
                if (bd.pairB() > pkt.first.value()) {
                    pairB = bd.pairB();
                    kicker = pkt.first.value();
                    
                    // Find one low value
                    auto itR = stats.crbegin();
                    CardVal lowValA = Card::lowAce;
                    while (
                        itR != stats.crend()
                     && (
                            lowValA == Card::lowAce
                         || lowValA == bd.pairA()
                         || lowValA == pairB
                        )
                    ) {
                        lowValA = itR->value();
                        ++itR;
                    }
                    if (kicker <= lowValA) {
                        kicker = Card::unknownValue;
                    }
                } else {
                    kicker = Card::unknownValue;
                    pairB = pkt.first.value();
                }

                return HandType(
                    HtTwoPair,
                    bd.pairA(),
                    pairB,
                    kicker,
                    Card::unknownValue
                );
            }
            return HandType(
                HtTwoPair,
                bd.pairA(),
                pkt.first.value(),
                noKickers
            );
        }
        CardVal pairA = Card::lowAce;
        CardVal pairB = Card::lowAce;
        short nKickers = 2;
        for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
            if (it->nCards() == 2) {
                if (pairA == Card::lowAce) {
                    pairA = it->value();
                    continue;
                } else {
                    pairB = it->value();
                    break;
                }
            } else if (pkt.has(it->value())) {
                if (pairA == Card::lowAce) {
                    pairA = it->value();
                    --nKickers;
                    continue;
                } else {
                    pairB = it->value();
                    --nKickers;
                    break;
                }
            }
        }
        if (pairB != Card::lowAce) {
            PktVals kickers = noKickers;
            switch (nKickers) {
            case 2: {
                // Find one high value
                auto itH = stats.cbegin();
                CardVal hValA = Card::lowAce;
                while (
                    itH != stats.cend()
                 && (
                        hValA == Card::lowAce
                     || hValA == pairA
                     || hValA == pairB
                    )
                ) {
                    hValA = itH->value();
                    ++itH;
                }
                if (pkt.first.value() > pkt.second.value()) {
                    kickers.first = pkt.first.value();
                } else {
                    kickers.first = pkt.second.value();
                }

                // Check one kicker
                if (kickers.first <= hValA) {
                    kickers.first = Card::unknownValue;
                }
                break;
            }
            case 1: {
                // Find one high value
                auto itH = stats.cbegin();
                CardVal hValA = Card::lowAce;
                while (
                    itH != stats.cend()
                 && (
                        hValA == Card::lowAce
                     || hValA == pairA
                     || hValA == pairB
                    )
                ) {
                    hValA = itH->value();
                    ++itH;
                }
                
                if (pkt.first.value() != pairA && pkt.first.value() != pairB) {
                    kickers.first = pkt.first.value();
                } else {
                    kickers.first = pkt.second.value();
                }
                
                // Check one kicker
                if (kickers.first <= hValA) {
                    kickers.first = Card::unknownValue;
                }
                break;
            }
            default: {
                break;
            } // end default
            } // end switch
            return HandType(
                HtTwoPair,
                pairA,
                pairB,
                kickers
            );
        }
    }

    // Check for a pair
    if (pkt.pairs()) {
        return HandType
        (
            HtPair,
            pkt.first.value(),
            Card::unknownValue,
            noKickers
        );
    }
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        if (it->nCards() == 2) {
            // Pair is on the board, pocket is free
            
            // Find two low values
            auto itR = stats.crbegin();
            CardVal lowValA = Card::lowAce;
            while (
                itR != stats.crend()
             && (lowValA == Card::lowAce || lowValA == it->value())
            ) {
                lowValA = itR->value();
                ++itR;
            }
            CardVal lowValB = Card::lowAce;
            while (
                itR != stats.crend()
             && (lowValB == Card::lowAce || lowValB == it->value())
            ) {
                lowValB = itR->value();
                ++itR;
            }
            PktVals kickers;
            if (pkt.first.value() > pkt.second.value()) {
                kickers.first = pkt.first.value();
                kickers.second = pkt.second.value();
            } else {
                kickers.first = pkt.second.value();
                kickers.second = pkt.first.value();
            }

            // Check two kickers            
            if (kickers.first < lowValA) {
                kickers = noKickers;
            } else if (kickers.first < lowValB || kickers.second < lowValA) {
                kickers.second = Card::unknownValue;
            }
            return HandType(
                HtPair,
                it->value(),
                Card::unknownValue,
                kickers
            );
        } else if (pkt.has(it->value())) {
            // Paired with single, one pocket card is free

            // Find one low values
            auto itR = stats.crbegin();
            CardVal lowValA = Card::lowAce;
            while (
                itR != stats.crend()
             && (lowValA == Card::lowAce || lowValA == it->value())
            ) {
                lowValA = itR->value();
                ++itR;
            }
            
            CardVal kicker;
            if (pkt.first.value() == it->value()) {
                kicker = pkt.second.value();
            } else {
                kicker = pkt.first.value();
            }

            if (kicker <= lowValA) {
                kicker = Card::unknownValue;
            }
            return HandType(
                HtPair,
                it->value(),
                Card::unknownValue,
                kicker,
                Card::unknownValue
            );
        }
    }

    // Check for high card
    {
        CardVal highVal = stats.front().value();

        // Find two low values, no iterator checking
        auto itR = stats.crbegin();
        CardVal lowValA = itR->value();
        ++itR;
        CardVal lowValB = itR->value();
        
        PktVals kickers;
        if (pkt.first.value() > pkt.second.value()) {
            kickers.first = pkt.first.value();
            kickers.second = pkt.second.value();
        } else {
            kickers.first = pkt.second.value();
            kickers.second = pkt.first.value();
        }

        if (kickers.first > highVal) {
            highVal = kickers.first;
            kickers.first = kickers.second;
            kickers.second = Card::unknownValue;
        }
        
        // Check two kickers
        if (kickers.first < lowValA) {
            kickers = noKickers;
        } else if (kickers.first < lowValB || kickers.second < lowValA) {
            kickers.second = Card::unknownValue;
        }

        return HandType(
            HtHighCard,
            highVal,
            Card::unknownValue,
            kickers
        );
    }
}

// ****** END ****** //
