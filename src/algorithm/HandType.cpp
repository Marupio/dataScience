#include<iterator>
#include<HandType.h>
#include<error.h>

// ****** Static Data Members ****** //

const std::array<std::string, 10> ds::HandType::HandTypeNames = {
    "Unknown",
    "HighCard",
    "Pair",
    "TwoPair",
    "Set",
    "Straight",
    "Flush",
    "FullHouse",
    "Four-of-a-kind",
    "StraightFlush"
};


// ****** Constructors ****** //

ds::HandType::HandType(const Board bd, const PktCards& pkt):
    ht_(HtUnknown),
    values_(PktVals::unknownValues)
{
    getHandType(bd, pkt);
}


// ****** Private Member Functions ****** //

void ds::HandType::setHandType(char t, PktVals v) {
    ht_ = t;
    values_ = v;
}


void ds::HandType::setHandType(char t, CardVal vA, CardVal vB) {
    ht_ = t;
    values_.first = vA;
    values_.second = vB;
}


void ds::HandType::getHandType(
    const Board& bd,
    const PktCards& pkt
) {
    const PktVals noKickers(Card::unknownValue, Card::unknownValue);

    // Check for straight flushes
    const Suit flushSuit = bd.flushSuit();
    const VecCardVal& flushValues(bd.flushValues());
    if (flushSuit != Card::unknownSuit) {
        const StraightCompleters straightFlushes(flushValues);
        for (
            auto sfit = straightFlushes.cbegin();
            sfit != straightFlushes.cend();
            ++sfit
        ) {
            PktCards testPkt(
                sfit->second.first, flushSuit,
                sfit->second.second, flushSuit
            );
            if (testPkt == pkt) {
                setHandType(HtStraightFlush, sfit->first, Card::unknownValue);
                return;
            }
        }
    }

    // Check for four-of-a-kind (FOAK)
    const VecValStats& stats(bd.stats());
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 4: {
            setHandType(HtFoak, it->value(), Card::unknownValue);
            return;
        }
        case 3: {
            // Set is on the board
            if (pkt.has(it->value()))
            {
                setHandType(HtFoak, it->value(), Card::unknownValue);
                return;
            }
            break;
        }
        case 2: {
            // Pair is on the board
            if (pkt.pairs(it->value())) {
                setHandType(HtFoak, it->value(), Card::unknownValue);
                return;
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
                    setHandType(HtFullHouse, it->value(), pairVal);
                    return;
                }
                // No pairs on board, must pair with pkt
                for (auto itp = stats.cbegin(); itp != stats.cend(); ++itp) {
                    if (pkt.has(itp->value())) {
                        setHandType(HtFullHouse, it->value(), itp->value());
                        return;
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
                            setHandType(
                                HtFullHouse,
                                it->value(),
                                itp->value()
                            );
                            return;
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
                    setHandType(HtFullHouse, it->value(), bd.pairA());
                    return;
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
    switch (flushValues.size()) {
    case 5: {
        CardVal highVal = bd.highestFlushValue();
        if (pkt.first.suit() == flushSuit) {
            highVal = std::max(pkt.first.value(), highVal);
        }
        if (pkt.second.suit() == flushSuit) {
            highVal = std::max(pkt.second.value(), highVal);
        }
        setHandType(HtFlush, highVal, Card::unknownValue);
        return;
    }
    case 4: {
        if (pkt.has(flushSuit)) {
            CardVal highVal = bd.highestFlushValue();
            if (pkt.first.suit() == flushSuit) {
                highVal = std::max(pkt.first.value(), highVal);
            }
            if (pkt.second.suit() == flushSuit) {
                highVal = std::max(pkt.second.value(), highVal);
            }
            setHandType(HtFlush, highVal, Card::unknownValue);
            return;
        }
        break;
    }
    case 3: {
        if (pkt.suited(flushSuit)) {
            CardVal highVal =
                std::max(bd.highestFlushValue(), pkt.first.value());
            highVal = std::max(highVal, pkt.second.value());
            setHandType(HtFlush, highVal, Card::unknownValue);
            return;
        }
        break;
    }
    case 0: {
        // No flush values
        break;
    }
    default: {
        FatalError << "Unexpected number of flush value cards on board. Flush "
            << "values are:\n" << flushValues << std::endl;
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
                setHandType(HtStraight, it->first, Card::unknownValue);
                return;
            }
        }
    }

    // Check for three-of-a-kind (set)
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 3: {
            // Set is on board, look for two kickers
            setHandType(HtSet, it->value(), Card::unknownValue);
            return;
        }
        case 2: {
            // Set uses a pair on the board, one pocket card is free
            if (pkt.has(it->value())) {
                setHandType(HtSet, it->value(), Card::unknownValue);
                return;
            }
            break;
        }
        case 1: {
            // Set uses a single on the board, pocket pairs required
            if (pkt.pairs(it->value())) {
                setHandType(HtSet, it->value(), Card::unknownValue);
                return;
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
                setHandType(HtTwoPair, pkt.first.value(), bd.pairA());
                return;
            }
            CardVal pairB = std::max(bd.pairB(), pkt.first.value());
            setHandType(HtTwoPair, bd.pairA(), pairB);
            return;
        }
        CardVal pairA = Card::lowAce;
        for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
            if (it->nCards() == 2) {
                if (pairA == Card::lowAce) {
                    pairA = it->value();
                    continue;
                } else {
                    setHandType(HtTwoPair, pairA, it->value());
                    return;
                }
            } else if (pkt.has(it->value())) {
                if (pairA == Card::lowAce) {
                    pairA = it->value();
                    continue;
                } else {
                    setHandType(HtTwoPair, pairA, it->value());
                    return;
                }
            }
        }
    }

    // Check for a pair
    if (pkt.pairs()) {
        setHandType(HtPair, pkt.first.value(), Card::unknownValue);
        return;
    }
    if (bd.pairA() != Card::unknownValue) {
        setHandType(HtPair, bd.pairA(), Card::unknownValue);
        return;
    }
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        if (pkt.has(it->value())) {
            setHandType(HtPair, it->value(), Card::unknownValue);
            return;
        }
    }

    // Check for high card
    CardVal highVal = std::max(
         bd.highestValue(), pkt.highestValue()
    );
    setHandType(HtHighCard, highVal, Card::unknownValue);
    return;
}



// ****** END ****** //
