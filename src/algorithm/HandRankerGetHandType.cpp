#include<PktMask.h>

// ****** Public Member Functions ****** //

ds::HandRanker::HandTypeStruct ds::HandRanker::getHandType
(
    const Board& bd,
    const PktCards& pkt
) {
    const PktVals noKickers(Card::unknownValue, Card::unknownValue);

    // Check for straight flushes
    const Suit flushSuit = bd.flushSuit();
    const VecCardVal& flushVals(bd.flushVals());
    if (flushSuit != Card::unknownSuit) {
        const StraightCompleters straightFlushes(
            findStraightCompleters(flushVals)
        );
        for (
            auto sfit = straightFlushes.cbegin();
            sfit != straightFlushes.cend();
            ++sfit
        ) {
            PktCards testPkt(
                sfit->second.first,
                flushSuit,
                sfit->second.second,
                flushSuit
            );
            if (testPkt == pkt) {
                return HandTypeStruct(
                    HtStraightFlush,
                    sfit->first,
                    Card::unknownValue.
                    noKickers
                );
            }
        }
    }

    // Check for four-of-a-kind (FOAK)
    const VecValStats& stats(bd.stats());
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 4: {
            CardVal kicker = std::max(pkt.first.value(), pkt.second.value());

            // Find one low value
            auto itR = stats.crbegin();
            CardVal lowVal = itR->value();
            if (lowVal == it->value()) {
                itR++;
                if (itR != stats.crend()) {
                    lowVal = itR->value();
                } else {
                    lowVal = Card::lowAce;
                }
            }

            kicker = kicker > lowVal ? kicker : Card::unknownValue;
            return HandTypeStruct(
                HtFoak,
                it->value(),
                Card::unknownValue,
                kicker,
                Card::unknownValue
            );
            break;
        }
        case 3: {
            // Set is on the board
            if (pkt.has(it->value()))
            {
                // Find one low value
                auto itR = stats.crbegin();
                CardVal lowVal = itR->value();
                if (lowVal == it->value()) {
                    itR++;
                    if (itR != stats.crend()) {
                        lowVal = itR->value();
                    } else {
                        lowVal = Card::lowAce;
                    }
                }
                CardVal kicker = pkt.first.value();
                if (kicker == it->value()) {
                    kicker = pkt.second.value();
                }
                if (kicker <= lowVal) {
                    kicker = Card::unknownValue;
                }

                return HandTypeStruct(
                    HtFoak,
                    it->value(),
                    Card::unknownValue,
                    kicker,
                    Card::unknownValue
                );
            }
            break;
        }
        case 2: {
            // Pair is on the board
            PktCards foakPkt;
            if (it->value() == bd.pairA()) {
                foakPkt = PktCards(
                    it->value(),
                    bd.pairAMissingSuits().first,
                    it->value(),
                    bd.pairAMissingSuits().second
                );
            } else {
                // Must be pairB
                #ifdef DSDEBUG
                    if (it->value() != bd.pairB()) {
                        FatalError << "Board stats and derived data do not "
                            << "agree. Pair value " << it->value() << " "
                            << "expected, but board pairA = " << bd.pairA()
                            << ", and pairB = " << bd.pairB() << std::endl;
                        abort();
                    }
                #endif
                foakPkt = PktCards(
                    it->value(),
                    bd.pairBMissingSuits().first,
                    it->value(),
                    bd.pairBMissingSuits().second
                );
            }
            if (foakPkt == pkt) {
                return HandTypeStruct(
                    HtFoak,
                    it->value(),
                    Card::unknownValue,
                    noKickers
                );
            }
            break;
        }
        default: {
            break;
        } // end default
        } // end switch
    }    

    // Check for full house
    if (
        bd.pairA() != Card::unknownValue
     || bd.pairB() != Card::unknownValue
     || bd.set() != Card::unknownValue
    ) {
        // Full house is possible
        for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
            // Look for highest set
            switch (it->nCards()) {
            case 3: {
                // FH set is on the board, pocket is free
                if (pkt.pairs() || bd.pairA() != Card::unknownValue) {
                    CardVal pairVal = std::max(pkt.first.value(), bd.pairA());
                    return HandTypeStruct(
                        HtFullHouse,
                        it->value(),
                        pairVal,
                        noKickers
                    );
                }
                for (auto itp = stats.cbegin(); itp != stats.cend(); ++itp) {
                    if (pkt.has(itp->value())) {
                        return HandTypeStruct(
                            HtFullHouse,
                            it->value(),
                            itp->value(),
                            noKickers
                        );
                    }
                }
                break;
            }
            case 2: {
                // FH set uses a pair on the board, needs one card in hand
                // setVal = it->value
                if (pkt.has(it->value())) {
                    for (
                        auto itp = stats.cbegin();
                        itp != stats.cend();
                        ++itp
                    ) {
                        if (itp == it) {
                            continue;
                        }
                        if (itp->nCards() >= 2 || pkt.has(itp->value())) {
                            return HandTypeStruct(
                                HtFullHouse,
                                it->value(),
                                itp->value(),
                                noKickers
                            );
                        }
                    }
                }
                break;
            }
            case 1: {
                // single is the set, needs pocket pairs and pair on the board                
                if (
                    bd.pairA() != Card::unknownValue
                 && pkt.pairs(it->value())
                ) {
                    return HandTypeStruct(
                        HtFullHouse,
                        it->value(),
                        bd.pairA(),
                        noKickers
                    );
                }
                break;
            }
            default: {
                break;
            } // end break
            } // end switch
        }
    }

    // Check for flush
    switch (flushVals.size()) {
    case 3: {
        if (!pkt.suited(flushSuit)) {
            break;
        }
        // fall through
    }
    case 4: {
        if (!pkt.has(flushSuit)) {
            break;
        }
        // fall through
    }
    case 5: {
        CardVal highVal = flushVals.front();
        PktVals kickers(noKickers);
        if (pkt.first.suit() == flushSuit) {
            kickers.first = pkt.first.value();
            if (pkt.first.value() > highVal) {
                highVal = pkt.first.value();
            }
        }
        if (pkt.second.suit() == flushSuit) {
            kickers.second = pkt.second.value();
            if (pkt.second.value() > highVal) {
                highVal = pkt.second.value();
            }
        }
        if (kickers.first > kickers.second) {
            CardVal temp = kickers.first;
            kickers.first = kickers.second;
            kickers.second = temp;
        }

        // Find two low values - no iterator checking
        auto itR = flushVals.crbegin();
        CardVal lowValA = itR->value();
        ++itR;
        CardVal lowValB = itR->value();

        // Check two kickers
        if (kickers.first < lowValA) {
            kickers = noKickers;
        } else if (kickers.first < lowValB || kickers.second < lowValA) {
            kickers.second = Card::unknownValue;
        }
        return HandTypeStruct(HtFlush, highVal, Card::unknownValue, kickers);
        break;
    }
    case 0: {
        // No flush values
        break;
    }
    default: {
        FatalError << "Unexpected number of flush value cards on board. Flush "
            << "values are:\n" << flushVals << std::endl;
        abort();
    } // end default
    } // end switch

    // Check for straight
    {
        // TODO - Implement vector copy using STL
        VecCardVal values;
        values.reserve(stats.size());
        for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
            values.push_back(it->value());
        }
        const StraightCompleters straights(findStraightCompleters(values));
        for (
            auto it = straights.cbegin();
            it != straights.cend();
            ++it
        ) {
            PktCards testPkt(
                it->second.first,
                Card::wildSuit,
                it->second.second,
                Card::wildSuit
            );
            if (testPkt == pkt) {
                return HandTypeStruct(
                    HtStraight,
                    it->first,
                    Card::unknownValue,
                    noKickers
                );
            }
        }
    }

    // Check for three-of-a-kind (set)
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 3: {
            // Set is on board, look for two kickers
            
            // Find two low values
            auto itR = stats.crbegin();
            CardVal lowValA = Card::lowAce;
            while (
                itR != stats.crend()
             && (lowValA != Card::lowAce || lowValA != it->value())
            ) {
                lowValA = itR->value();
                ++itR;
            }
            CardVal lowValB = Card::lowAce;
            while (
                itR != stats.crend()
             && (lowValB != Card::lowAce || lowValB != it->value())
            ) {
                lowValB = itR->value();
                ++itR;
            }

            PktCards kickers;
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
            
            return HandTypeStruct(
                HtSet,
                it->value(),
                Card::unknownValue,
                kickers
            );
            break;
        }
        case 2: {
            // Set uses a pair on the board, one pocket card is free
            if (pkt.has(it->value())) {

                // Find one low value
                auto itR = stats.crbegin();
                CardVal lowValA = Card::lowAce;
                while (
                    itR != stats.crend()
                 && (lowValA != Card::lowAce || lowValA != it->value())
                ) {
                    lowValA = itR->value();
                    ++itR;
                }

                CardVal kicker =
                    pkt.first.value() == it->value()
                  ? pkt.second.value()
                  : pkt.first.value();
                
                if (kicker < lowValA) {
                    kicker = Card::unknownValue;
                }

                return HandTypeStruct(
                    HtSet,
                    it->value(),
                    Card::unknownValue,
                    kicker,
                    Card::unknownValue
                );
            }
            break;
        }
        case 1: {
            // Set uses a single on the board, pocket pairs required
            if (pkt.pairs(it->value())) {
                return HandTypeStruct
                (
                    HtSet,
                    it->value(),
                    Card::unknownValue,
                    noKickers
                );
            }
            break;
        }
        default: {
            FatalError << "Unexpected nCards when checking for Set. nCards "
                << "is: " << it->nCards() << std::endl;
            abort();
        } // end default
        } // end switch
    }    

    // Check for two pair
    if (bd.pairA() != Card::unknownValue) {
        if (pkt.pairs()) {
            if (pkt.first.value() > bd.pairA()) {
                return HandTypeStruct(
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
                            lowValA != Card::lowAce
                         || lowValA != bd.pairA()
                         || lowValA != pairB
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

                return HandTypeStruct(
                    HtTwoPair,
                    bd.pairA(),
                    pairB,
                    kicker,
                    Card::unknownValue
                );
            }
            return HandTypeStruct(
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
                    break;
                }
            }
        }
        if (pairB != Card::lowAce) {
            PktVals kickers = noKickers;
            switch (nKickers) {
            case 2: {
                // Find one low value
                auto itR = stats.crbegin();
                CardVal lowValA = Card::lowAce;
                while (
                    itR != stats.crend()
                 && (
                        lowValA != Card::lowAce
                     || lowValA != pairA
                     || lowValA != pairB
                    )
                ) {
                    lowValA = itR->value();
                    ++itR;
                }

                if (pkt.first.value() > pkt.second.value()) {
                    kickers.first = pkt.first.value();
                } else {
                    kickers.first = pkt.second.value();
                }

                // Check one kicker
                if (kickers.first <= lowValA) {
                    kickers.first = Card::unknownValue;
                }
                break;
            }
            case 1: {
                // Find two low values
                auto itR = stats.crbegin();
                CardVal lowValA = Card::lowAce;
                while (
                    itR != stats.crend()
                 && (
                        lowValA != Card::lowAce
                     || lowValA != pairA
                     || lowValA != pairB
                    )
                ) {
                    lowValA = itR->value();
                    ++itR;
                }
                CardVal lowValB = Card::lowAce;
                while (
                    itR != stats.crend()
                 && (
                        lowValB != Card::lowAce
                     || lowValB != pairA
                     || lowValB != pairB
                    )
                ) {
                    lowValB = itR->value();
                    ++itR;
                }
                
                if (pkt.first.value() != pairA && pkt.first.value() != pairB) {
                    kickers.first = pkt.first.value();
                } else {
                    kickers.first = pkt.second.value();
                }
                
                // Check one kicker
                if (kickers.first <= lowValB) {
                    kickers.first = Card::unknownValue;
                }
                break;
            }
            default: {
                break;
            } // end default
            } // end switch
            return HandTypeStruct(
                HtTwoPair,
                pairA,
                it->value(),
                kickers
            );
        }
    }

    // Check for a pair
    if (pkt.pairs()) {
        return HandTypeStruct
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
             && (lowValA != Card::lowAce || lowValA != it->value())
            ) {
                lowValA = itR->value();
                ++itR;
            }
            CardVal lowValB = Card::lowAce;
            while (
                itR != stats.crend()
             && (lowValB != Card::lowAce || lowValB != it->value())
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
            return HandTypeStruct(
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
             && (lowValA != Card::lowAce || lowValA != it->value())
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
            return HandTypeStruct(
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
        CardVal lowValA = *itR;
        ++itR;
        CardVal lowValB = *itR;
        
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

        return HandTypeStruct(
            HtHighCard,
            highVal,
            Card::unknownValue,
            kickers
        );
    }
}

// ****** END ****** //
