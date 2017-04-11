#include<PktMask.h>

// ****** Public Member Functions ****** //

ds::HandRanker::HandTypeStruct ds::HandRanker::getHandType
(
    const Board& bd,
    const PktCards& pkt
) {
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
                return HandTypeStruct(HtStraightFlush, sfit->first);
            }
        }
    }

    // Check for four-of-a-kind (FOAK)
    const VecValStats& stats(bd.stats());
    #ifdef DSDEBUG
        if (stats.nCards() < 3) {
            FatalError << "Card value stats array has an unexpected size: "
                << stats.nCards() << std::endl;
            abort();
        }
    #endif
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 4: {
            return HandTypeStruct(HtFoak, it->value());
            break;
        }
        case 3: {
            // TOAK is on the board
            const Card toakCard(it->value(), bd.toakMissingSuit());
            PktCards foakPkt(toakCard, Card::wildCard);
            if (pkt == foakPkt) {
                // Player has FOAK
                return HandTypeStruct(HtFoak, it->value());
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
                return HandTypeStruct(HtFoak, it->value());
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
     || bd.toak() != Card::unknownValue
    ) {
        // Full house is possible
        for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
            // Look for highest set
            switch (it->nCards()) {
            case 3: {
                // FH set is on the board, pocket is free
                if (pkt.pairs() || bd.pairA() != Card::unknownValue) {
                    return HandTypeStruct(HtFullHouse, it->value());
                }
                for (auto itp = stats.cbegin(); itp != stats.cend(); ++itp) {
                    if (pkt.has(itp->value())) {
                        return HandTypeStruct(HtFullHouse, it->value());
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
                            // Pair is on the board
                            return HandTypeStruct(HtFullHouse, it->value());
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
                    return HandTypeStruct(HtFullHouse, it->value());
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
    case 5:
    {
        CardVal highVal = stats.front().value();
        if (pkt.first.suit() == flushSuit && pkt.first.value() > highVal) {
            highVal = pkt.first.value();
        }
        if (pkt.second.suit() == flushSuit && pkt.second.value() > highVal) {
            highVal = pkt.second.value();
        }
        return HandTypeStruct(HtFlush, highVal);
        break;
    }
    case 4: {
        if (pkt.has(flushSuit)) {
            CardVal highVal = stats.front().value();
            if (pkt.first.suit() == flushSuit && pkt.first.value() > highVal) {
                highVal = pkt.first.value();
            }
            if (
                pkt.second.suit() == flushSuit && pkt.second.value() > highVal
            ) {
                highVal = pkt.second.value();
            }
            return HandTypeStruct(HtFlush, highVal);
        }
        break;
    }
    case 3: {
        if (pkt.suited(flushSuit)) {
            CardVal highVal = stats.front().value();
            highVal = std::max(highVal, pkt.first.value());
            highVal = std::max(highVal, pkt.second.value());
            return HandTypeStruct(HtFlush, highVal);
        }
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
                return HandTypeStruct(HtStraight, it->first);
            }
        }
    }

    // Check for three-of-a-kind (TOAK)
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 3: {
            // TOAK is on board, look for two kickers
            return HandTypeStruct(HtToak, it->value());
            break;
        }
        case 2: {
            // TOAK uses a pair on the board, one pocket card is free
            if (pkt.has(it->value())) {
                return HandTypeStruct(HtToak, it->value());
            }
            break;
        }
        case 1: {
            // TOAK uses a single on the board, pocket pairs required
            if (pkt.pairs(it->value())) {
                return HandTypeStruct(HtToak, it->value());
            }
            break;
        }
        default: {
            FatalError << "Unexpected nCards when checking for TOAK. nCards "
                << "is: " << it->nCards() << std::endl;
            abort();
        } // end default
        } // end switch
    }    

    // Check for two pair
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 2: {
            // One pair is on the board. Second pair can be pocket pairs or
            // paired with another single
            if (pkt.pairs()) {
                return HandTypeStruct(HtTwoPair, it->value());
            }
            for (auto itB = stats.cbegin(); itB != stats.cend(); ++itB) {
                if (itB == it) {
                    continue;
                }
                if (pkt.has(itB->value())) {
                    return HandTypeStruct(HtTwoPair, it->value());
                }
            }
            break;
        }
        case 1: {
            // Pair A uses single from board
            if (pkt.has(it->value())) {
                for (auto itB = stats.cbegin(); itB != stats.cend(); ++itB) {
                    if (itB == it) {
                        continue;
                    }
                    if (pkt.has(itB->value())) {
                        return HandTypeStruct(HtTwoPair, it->value());
                    }
                }
            }
            break;
        }
        default: {
            FatalError << "Unexpected nCards when checking for two pairs. "
                << "nCards is: " << it->nCards() << std::endl;
            abort();
            break;
        } // end break
        } // end switch
    }

    // Check for a pair
    if (pkt.pairs()) {
        return HandTypeStruct(HtPair, pkt.first.value());
    }
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        if (pkt.has(it->value())) {
            return HandTypeStruct(HtPair, it->value());
        }
    }

    // Check for high card
    {
        CardVal highVal = stats.front().value();
        highVal = std::max(highVal, pkt.first.value());
        highVal = std::max(highVal, pkt.second.value());
        return HandTypeStruct(HtHighCard, highVal);
    }
}

// ****** END ****** //
