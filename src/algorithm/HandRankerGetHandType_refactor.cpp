#include<PktMask.h>

// ****** Public Member Functions ****** //

ds::HandRanker::HandType ds::HandRanker::getHandType
(
    const Board& bd,
    const PktCards& pkt
) {
    const PktVals noKickers(Card::unknownValue, Card::unknownValue);

    // Check for straight flushes
    const Suit flushSuit = bd.flushSuit();
    const VecCardVal& flushVals(bd.flushVals());
    if (flushSuit != Card::unknownSuit) {
        const StraightCompleters straightFlushes(flushVals);
        for (
            auto sfit = straightFlushes.cbegin();
            sfit != straightFlushes.cend();
            ++sfit
        ) {
            PktCards testPkt(
                sfit->second.first, flushSuit,
                sfit->second.second, flushSuit
            );
            if (noWildEquals(testPkt, pkt)) {
                return HandType(
                    HtStraightFlush,
                    sfit->first,
                    Card::unknownValue
                );
            }
        }
    }

    // Check for four-of-a-kind (FOAK)
    const VecValStats& stats(bd.stats());
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 4: {
            return HandType(HtFoak, it->value(), Card::unknownValue);
            break;
        }
        case 3: {
            // Set is on the board
            if (pkt.has(it->value()))
            {
                return HandType(HtFoak, it->value(), Card::unknownValue);
            }
            break;
        }
        case 2: {
            // Pair is on the board
            if (pkt.pairs(it->value()) {
                return HandType(HtFoak, it->value(), Card::unknownValue);
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
                    const CardVal pairVal (
                        pkt.pairs()
                      ? std::max(pkt.first.value(), bd.pairA())
                      : bd.pairA()
                    );
                    return HandType(HtFullHouse, it->value(), pairVal);
                }
                // No pairs on board, must pair with pkt
                for (auto itp = stats.cbegin(); itp != stats.cend(); ++itp) {
                    if (pkt.has(itp->value())) {
                        return HandType(HtFullHouse, it->value(), itp->value());
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
                            return HandType(
                                HtFullHouse,
                                it->value(),
                                itp->value()
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
                    return HandType(HtFullHouse, it->value(), bd.pairA());
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
    case 5: {
        CardVal highVal = bd.highFlushVal();
        if (pkt.first.suit() == flushSuit) {
            highVal = std::max(pkt.first.value(), highVal);
        }
        if (pkt.second.suit() == flushSuit) {
            highVal = std::max(pkt.secondValue(), highVal);
        }
        return HandType(HtFlush, highVal, Card::unknownValue);
        break;
    }
    case 4: {
        if (pkt.has(flushSuit)) {
            CardVal highVal = bd.highFlushVal();
            if (pkt.first.suit() == flushSuit) {
                highVal = std::max(pkt.first.value(), highVal);
            }
            if (pkt.second.suit() == flushSuit) {
                highVal = std::max(pkt.secondValue(), highVal);
            }
            return HandType(HtFlush, highVal, Card::unknownValue);
        }
        break;
    }
    case 3: {
        if (pkt.suited(flushSuit)) {
            CardVal highVal = std::max(bd.highFlushVal(), pkt.firstValue());
            highVal = std::max(highVal, pkt.secondVal());
            return HandType(HtFlush, highVal, Card::unknownValue);
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
        const StraightCompleters straights(stats);
        for (
            auto it = straights.cbegin();
            it != straights.cend();
            ++it
        ) {
            PktCards testPkt(
                it->second.first, Card::wildSuit,
                it->second.second, Card::wildSuit
            );
            if (testPkt == pkt) {
                return HandType(HtStraight, it->first, Card::unknownValue);
            }
        }
    }

    // Check for three-of-a-kind (set)
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 3: {
            // Set is on board, look for two kickers
            return HandType(HtSet, it->value(), Card::unknownValue);
            break;
        }
        case 2: {
            // Set uses a pair on the board, one pocket card is free
            if (pkt.has(it->value())) {
                return HandType(HtSet, it->value(), Card::unknownValue);
            }
            break;
        }
        case 1: {
            // Set uses a single on the board, pocket pairs required
            if (pkt.pairs(it->value())) {
                return HandType(HtSet, it->value(), Card::unknownValue);
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
                return HandType(HtTwoPair, pkt.first.value(), bd.pairA());
            }
            CardVal pairB = std::max(bd.pairB(), pkt.first.value());
            return HandType(HtTwoPair, bd.pairA(), pairB);
        }
        CardVal pairA = Card::lowAce;
        for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
            if (it->nCards() == 2) {
                if (pairA == Card::lowAce) {
                    pairA = it->value();
                    continue;
                } else {
                    return HandType(HtTwoPair, pairA, it->value());
                }
            } else if (pkt.has(it->value())) {
                if (pairA == Card::lowAce) {
                    pairA = it->value();
                    continue;
                } else {
                    return HandType(HtTwoPair, pairA, it->value());
                }
            }
        }
    }

    // Check for a pair
    if (pkt.pairs()) {
        return HandType(HtPair, pkt.first.value(), Card::unknownValue);
    }
    if (bd.pairA() != Card::unknownValue) {
        return HandType(HtPair, bd.pairA(), Card::unknownValue);
    }
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        if (pkt.has(it->value())) {
            return HandType(HtPair, it->value(), Card::unknownValue);
        }
    }

    // Check for high card
    CardVal highVal = std::max(
         bd.highestVal(), pkt.highestVal()
    );
    return HandType(HtHighCard, highValue, Card::unknownValue);
}

// ****** END ****** //
