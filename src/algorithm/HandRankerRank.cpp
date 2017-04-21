#include<Avoiders.h>

// ****** Public Member Functions ****** //

short ds::HandRanker::rank() {
    short rank = 0;
    mask_ = baskMask_;

    // Check for straight flushes
    const Suit flushSuit = cbd_.flushSuit();
    const VecCardVal& flushValues(cbd_.flushValues());
    if (flushSuit != Card::unknownSuit) {
        const StraightCompleters straightFlushes(flushValues);
        for (
            auto sfit = straightFlushes.cbegin();
            sfit != straightFlushes.cend();
            ++sfit
        ) {
            PktCards testPkt(
                sfit->second.first,
                sfit->second.first == Card::wildValue
                  ? Card::wildSuit
                  : flushSuit,
                sfit->second.second,
                sfit->second.second == Card::wildValue
                  ? Card::wildSuit
                  : flushSuit
            );
            if (testPkt == pkt_) {
                return rank;
            } else {
                rank += mask_.remove(testPkt);
            }
        }
    }

    // Check for four-of-a-kind
        // If FOAK showing on board:
            // rank by cards higher than fifth card (val wild)
            // then exit (wild wild)
        // If set showing on board (valA):
            // If pkt_ ! have valA, remove (valA, wild)
            // Otherwise:
                // Rank extra pocket card A..lowest on board (valA, val)
                // Below that (valA, wild)
        // If one or more pairs showing on board (valA) > (valB)
            // (valA valA) suits known
            // then (valB valB) suits known, if second pair is present
            // Continue
        // Check set and pairs for highest value
            // set - rank extra pocket card higher first, then wild
            // pairs - specific pkt_ only

    // Check for four-of-a-kind (FOAK)
    const VecValStats& stats(cbd_.stats());
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 4: {
            // FOAK is on the board
            // rank by cards higher than fifth card (val wild)
            // then exit (wild wild)
            // FFFF? ??

            // Search through kickers
            rankOneKicker(
                rank,
                VecCard(1, Card::wildCard),
                cbd_.highestValue(it->value()),
                AvoidValue(it->value())
            );
            return rank;
        }
        case 3: {
            // Set is on the board
            const Card setCard(it->value(), cbd_.setMissingSuit());
            if (pkt_.has(it->value())) {
                // Player has FOAK
                // FFF?? F?

                // Find highest kicker
                rankOneKicker(
                    rank,
                    VecCard(1, setCard),
                    cbd_.highestValue(it->value()),
                    AvoidValue(it->value())
                );
                return rank;
            } else {
                // Player does not have FOAK
                PktCards foakPkt(setCard, Card::wildCard);
                rank += mask_.remove(foakPkt);
            }
            break;
        }
        case 2: {
            // Pair is on the board
            PktCards foakPkt;
            if (it->value() == cbd_.pairA()) {
                foakPkt = PktCards(
                    it->value(),
                    cbd_.pairAMissingSuits().first,
                    it->value(),
                    cbd_.pairAMissingSuits().second
                );
            } else {
                // Must be pairB
                #ifdef DSDEBUG
                    if (it->value() != cbd_.pairB()) {
                        FatalError << "Board stats and derived data do not "
                            << "agree. Pair value " << it->value() << " "
                            << "expected, but board pairA = " << cbd_.pairA()
                            << ", and pairB = " << cbd_.pairB() << "\n" << cbd_
                            << " " << pkt_ << std::endl;
                        abort();
                    }
                #endif
                foakPkt = PktCards(
                    it->value(),
                    cbd_.pairBMissingSuits().first,
                    it->value(),
                    cbd_.pairBMissingSuits().second
                );
            }
            if (foakPkt == pkt_) {
                return rank;
            } else {
                rank += mask_.remove(foakPkt);
            }
            break;
        }
        default: {
            break;
        } // end default
        } // end switch
    }    

    // Check for full house
        // Set or pair must be present
        // Only sets possible are present in cbd_.values
        // Start with the highest and move down
        // If it's a set
            // Run through all other cards for pair:
                // (val val) if not present on board
                // (val wild) if present as a single
                // Exit with (wild wild) if present as a pair
        // If two pairs, then start with highest (call it valA)
            // Run through all other cbd_.values as (valA, val)
            // Once you reach the other pair, it's (valA, wild)
            // Then repeat with (valB)
        // If one pair
            // Run through cbd_.values for set first (valA)
                // If it is the pair:
                    // then run through cbd_.values for pair (valA val)
                // If it is single:
                    // pocket pairs only (valB valB)
        // Continue

    // Check for full house
    if (
        cbd_.pairA() != Card::unknownValue
     || cbd_.pairB() != Card::unknownValue
     || cbd_.set() != Card::unknownValue
    ) {
        // Full house is possible
        for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
            // Look for highest set
            switch (it->nCards()) {
            case 3: {
                // FH set is on the board, pocket is free
                auto itp = stats.cbegin();
                for (
                    CardVal pairVal = Card::ace;
                    pairVal > Card::lowAce;
                    --pairVal
                ) {
                    if (pairVal == it->value()) {
                        // Pair value can't be set value
                        if (itp == it) {
                            ++itp;
                        }
                        continue;
                    }
                    if (itp != stats.cend() && pairVal == itp->value()) {
                        // best pair is a value on the board
                        if (itp->nCards() > 1) {
                            // best pair is already paired on the board
                            // all remaining hands the same
                            return rank;
                        } else {
                            // It is a single
                            PktCards testPkt(
                                pairVal,
                                Card::wildSuit,
                                Card::wildCard
                            );
                            if (pkt_ == testPkt) {
                                return rank;
                            } else {
                                rank += mask_.remove(testPkt);
                            }
                        }
                        ++itp;
                    } else {
                        // Not on the board, must be pocket paired
                        PktCards testPkt(
                            pairVal,
                            Card::wildSuit,
                            pairVal,
                            Card::wildSuit
                        );
                        if (pkt_ == testPkt) {
                            return rank;
                        } else {
                            rank += mask_.remove(testPkt);
                        }
                    }
                }
                break;
            }
            case 2: {
                // FH set uses a pair on the board, needs one card in hand
                for (auto itp = stats.cbegin(); itp != stats.cend(); ++itp) {
                    if (itp == it) {
                        continue;
                    }
                    if (itp->nCards() > 1) {
                        // Pair is on the board
                        // SSPP? S?
                        PktCards testPkt(
                            it->value(),
                            Card::wildSuit,
                            Card::wildCard
                        );
                        if (pkt_ == testPkt) {
                            return rank;
                        } else {
                            rank += mask_.remove(testPkt);
                        }
                    } else {
                        // FH pair uses single from the board
                        // SSP?? SP
                        PktCards testPkt(
                            it->value(),
                            Card::wildSuit,
                            itp->value(),
                            Card::wildSuit
                        );
                        if (pkt_ == testPkt) {
                            return rank;
                        } else {
                            rank += mask_.remove(testPkt);
                        }
                    }
                }
                break;
            }
            case 1: {
                // single is the set, needs pocket pairs and pair on the board
                // SPP?? SS
                if (cbd_.pairA() != Card::unknownValue) {
                    PktCards testPkt(
                        it->value(),
                        Card::wildSuit,
                        it->value(),
                        Card::wildSuit
                    );
                    if (pkt_ == testPkt) {
                        return rank;
                    } else {
                        rank += mask_.remove(testPkt);
                    }
                }
                // No need to check pairB
                break;
            }
            default: {
                break;
            } // end break
            } // end switch
        }
    }

    // Check for flush
        // Five flush
            // Run through all higher (card card), then (card, wild), then
            // exit with (wild, wild)
        // Four flush
            // If player doesn't have (suit), remove (suit, wild)
            // Else Run through all higher (card card), then (card, wild)
            // Continue
        // Three flush
            // If player doesn't have (suit, suit), then remove (suit, suit)
            // and move on
            // Else move through existing ranks
            // Continue
    // Check for flush
    switch (flushValues.size()) {
    case 5: {
        // Look for two kickers
        rankKickersFiveFlush(rank);
        return rank;
    }
    case 4: {
        if (pkt_.has(flushSuit)) {
            rankKickersFourFlush(rank);
            return rank;
        } else {
            PktCards testPkt(Card::wildValue, flushSuit, Card::wildCard);
            rank += mask_.remove(testPkt);
        }
        break;
    }
    case 3: {
        if (pkt_.suited(flushSuit)) {
            rankKickersThreeFlush(rank);
            return rank;
        } else {
            PktCards testPkt(
                Card::wildValue,
                flushSuit,
                Card::wildValue,
                flushSuit
            );
            rank += mask_.remove(testPkt);
        }
        break;
    }
    case 0: {
        // No flush values
        break;
    }
    default: {
        FatalError << "Unexpected number of flush value cards on board. Flush "
            << "values are:\n" << flushValues << "\n" << cbd_ << " "
            << pkt_ << std::endl;
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
                it->second.first,
                Card::wildSuit,
                it->second.second,
                Card::wildSuit
            );
            if (testPkt == pkt_) {
                return rank;
            } else {
                rank += mask_.remove(testPkt);
            }
        }
    }
    
    // Check for sets
        // Run through all cbd_.value
            // If exists as set:
                // Run through (valA, valB) higher than lowest two on board
                // Then through (valA wild) higher than lowest on board
                // Then exit (wild wild)
            // If exists as pair (valA)
                // If pkt_ ! have valA, remove (valA, wild)
                // Otherwise run through (valA valB) valB > lowest on board
                // Continue
            // If exists as single (valA)
                // (valA valA) only
                // Contiune

    // Check for three-of-a-kind (set)
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 3: {
            // set is on board, look for two kickers

            // Look for two kickers
            const PktVals highVals(cbd_.highestTwoValues(it->value()));
            
            auto itA = stats.cbegin();
            for (
                CardVal kickerA = Card::ace;
                kickerA > highVals.first;
                --kickerA
            ) {
                if (itA != stats.cend() && kickerA == itA->value()) {
                    ++itA;
                    continue;
                }
                if (kickerA == it->value()) {
                    continue;
                }
                auto itB = itA;
                for (
                    CardVal kickerB = kickerA - 1;
                    kickerB > highVals.second;
                    --kickerB
                ) {
                    if (itB != stats.cend() && kickerB == itB->value()) {
                        ++itB;
                        continue;
                    }
                    if (kickerB == it->value()) {
                        continue;
                    }
                    PktCards testPkt(
                        kickerA,
                        Card::wildSuit,
                        kickerB,
                        Card::wildSuit
                    );
                    if (pkt_ == testPkt) {
                        return rank;
                    } else {
                        rank += mask_.remove(testPkt);
                    }
                }
                // kickerB too low, continue to save iterations
                short rankOffset = 0;
                for (
                    CardVal kickerB = highVals.second - 1;
                    kickerB > Card::lowAce;
                    --kickerB
                ) {
                    if (itB != stats.cend() && kickerB == itB->value()) {
                        ++itB;
                        continue;
                    }
                    if (kickerB == it->value()) {
                        continue;
                    }
                    PktCards testPkt(
                        kickerA,
                        Card::wildSuit,
                        kickerB,
                        Card::wildSuit
                    );
                    if (pkt_ == testPkt) {
                        return rank;
                    } else {
                        rankOffset += mask_.remove(testPkt);
                    }
                }
                rank += rankOffset;
            }

            // kickerA could still be higher than highVals.second
            for (
                CardVal kickerA = highVals.first - 1;
                kickerA > highVals.second;
                --kickerA
            ) {
                if (itA != stats.cend() && kickerA == itA->value()) {
                    ++itA;
                    continue;
                }
                if (kickerA == it->value()) {
                    continue;
                }
                auto itB = itA;
                short rankOffset = 0;
                for (
                    CardVal kickerB = kickerA - 1;
                    kickerB > Card::lowAce;
                    --kickerB
                ) {
                    if (itB != stats.cend() && kickerB == itB->value()) {
                        ++itB;
                        continue;
                    }
                    if (kickerB == it->value()) {
                        continue;
                    }
                    PktCards testPkt(
                        kickerA,
                        Card::wildSuit,
                        kickerB,
                        Card::wildSuit
                    );
                    if (pkt_ == testPkt) {
                        return rank;
                    } else {
                        rankOffset += mask_.remove(testPkt);
                    }
                }
                rank += rankOffset;
            }

            // kickerA too low all remaining hands the same
            return rank;
        }
        case 2: {
            // set uses a pair on the board, one pocket card is free
            const PktSuits& missingSuits (
                it->value() == cbd_.pairA()
              ? cbd_.pairAMissingSuits()
              : cbd_.pairBMissingSuits()
            );
            
            // We could use wildSuit, but we know what suits are missing, so
            // we specify the suits and check two cards to reduce effort
            VecCard setCards {
                Card(it->value(), missingSuits.first),
                Card(it->value(), missingSuits.second)
            };
            
            if (pkt_.has(it->value()))
            {
                // Player has set
                // SS??? S?
                // search through kickers
                rankOneKicker(
                    rank,
                    setCards,
                    cbd_.highestTwoValues(it->value()).second,
                    AvoidValue(it->value())
                );
                return rank;
            } else {
                // pkt_ does not have set
                #ifdef DS_DEBUG
                if (setCards.size() != 2) {
                    FatalError << "Expecting setCards size to be 2. Is "
                        << setCards.size() << ". setCards are:\n" << setCards
                        << "\n" << cbd_ << " " << pkt_ << std::endl;
                    abort();
                }
                #endif
                PktCards testPktA(setCards.front(), Card::wildCard);
                PktCards testPktB(setCards.back(), Card::wildCard);
                rank += mask_.remove(testPktA);
                rank += mask_.remove(testPktB);
            }
            break;
        }
        case 1: {
            // set uses a single on the board, pocket pairs required
            PktCards testPkt (
                it->value(),
                Card::wildSuit,
                it->value(),
                Card::wildSuit
            );
            if (pkt_ == testPkt) {
                return rank;
            } else {
                rank += mask_.remove(testPkt);
            }
            break;
        }
        default: {
            FatalError << "Unexpected nCards when checking for set. nCards "
                << "is: " << it->nCards() << "\n" << cbd_ << " " << pkt_
                << std::endl;
            abort();
        } // end default
        } // end switch
    }    

    // Check for two pair
        // Two pairs: valA > valB
        // One of the pairs must be made from board.values
        // ValA:
            // Run through Ace .. lowest board value (for pairA)
            // If valA is paired on board:
                // Run from valB=valA-1 .. 2
                // If valB is paired on board:
                    // Run kicker from A..highest on board not valA (kicker wild)
                    // Then exit (wild wild)
                // If valB is single on board:
                    // Run kicker from A..highest on board not valA (valB kicker)
                    // Then remove (valB wild), and Continue
                // If valB is not on board:
                    // remove (valB valB) and Continue
            // If valA is single on board:
                // Run valB over other board.values
                    // If valB is pair:
                        // Run kicker from A..highest on board not valA (valA kicker)
                        // Then remove (valA wild) and continue
                    // if valB is single:
                        // remove (valA valB) and continue
            // If valA is not on board:
                // If no pairs are present on board, no two pair exists
                // If pairs exist on board, remove (valA valA) and continue

    // Check for two pair
    if (cbd_.pairA() != Card::unknownValue) {
        // Pocket pair could be highest, have to iterate through all cards
        const CardVal boardPairA = cbd_.pairA();

        auto itA = stats.cbegin();

        for (CardVal pairA = Card::ace; pairA > boardPairA; --pairA) {
            if (itA != stats.cend() && itA->value() == pairA) {
                // pairA is on the board
                if (itA->nCards() == 2) {
                    // pairA is paired on the board, pairB can be pocket pairs
                    const CardVal lowest =
                        std::max(cbd_.lowestValue(itA->value()), cbd_.pairB());
                    auto itB = itA + 1;
                    for (CardVal pairB = pairA - 1; pairB > lowest; --pairB) {
                        if (itB != stats.cend() && itB->value() == pairB) {
                            // pairB is on the board
                            if (itB->value() == 2) {
                                // pairB is paired on the board
                                // AABB? ??
                                rankOneKicker(
                                    rank,
                                    VecCard(1, Card::wildCard),
                                    cbd_.highestValue(PktVals(pairA, pairB)),
                                    AvoidPktVals(pairA, pairB)
                                );
                                return rank;
                            } else {
                                // pairB is single on the board
                                // AAB?? B?
                                if (!pkt_.has(pairB)) {
                                    PktCards testPkt(
                                        pairB, Card::wildSuit,
                                        Card::wildCard
                                    );
                                    rank += mask_.remove(testPkt);
                                } else {
                                    rankOneKicker(
                                        rank,
                                        VecCard(1, Card(pairB, Card::wildSuit)),
                                        cbd_.highestValue(PktVals(pairA, pairB)),
                                        AvoidPktVals(pairA, pairB)
                                    );
                                    return rank;
                                }
                            }
                            ++itB;
                        }
                    }
                } else {
                    #ifdef DSDEBUG
                    if (itA->nCards() != 1) {
                        FatalError << "Expecting number of cards to be 1. Is "
                            << itA->nCards() << "\n" << cbd_ << " " << pkt_
                            << std::endl;
                        abort();
                    }
                    #endif
                    // pairA is single on the board, pairB must pair with board
                    auto itB = itA + 1;
                    for (; itB != stats.cend(); ++itB) {
                        if (itB->nCards() == 2) {
                            // pairB is paired on the board
                            // ABB?? A?  Look for kicker
                            if (!pkt_.has(itA->value())) {
                                PktCards testPkt(
                                    pairA, Card::wildSuit,
                                    Card::wildCard
                                );
                                rank += mask_.remove(testPkt);
                            } else {
                                rankOneKicker(
                                    rank,
                                    VecCard(1, Card(pairA, Card::wildSuit)),
                                    cbd_.lowestValue(
                                        PktVals(pairA, itB->value())
                                    ),
                                    AvoidPktVals(pairA, itB->value())
                                );
                                return rank;
                            }
                        } else {
                            // pairB is single on the board
                            // AB??? AB
                            PktCards testPkt(
                                pairA, Card::wildSuit,
                                itB->value(), Card::wildSuit
                            );
                            if (pkt_ == testPkt) {
                                return rank;
                            } else {
                                rank += mask_.remove(testPkt);
                            }
                        }
                    }
                }
                ++itA;
            } else {
                // pairA is not on the board, must be pocket pairs
                // BB??? AA
                PktCards testPkt(pairA, Card::wildSuit, pairA, Card::wildSuit);
                if (testPkt == pkt_) {
                    return rank;
                } else {
                    rank += mask_.remove(testPkt);
                }
            }
        }
        
        // boardPairA is pairA, pairB can be pocketPairs
        const CardVal pairA = boardPairA;
        auto itB = itA + 1;
        for (CardVal pairB = pairA - 1; pairB > Card::lowAce; --pairB) {
            if (itB != stats.cend() && itB->value() == pairB) {
                // pairB is on the board
                if (itB->nCards() == 2) {
                    // pairB is paired on the board, look for kickers
                    // AABB? ??
                    rankOneKicker(
                        rank,
                        VecCard(1, Card::wildCard),
                        cbd_.highestValue(PktVals(pairA, pairB)),
                        AvoidPktVals(pairA, pairB)
                    );
                    return rank;
                }
                
                #ifdef DSDEBUG
                if (itB->nCards() != 1) {
                    FatalError << "Expecting number of cards to be 1. Is "
                        << itB->nCards() << "\n" << cbd_ << " " << pkt_
                        << std::endl;
                    abort();
                }
                #endif
                // pairB is single on the board, look for one kicker
                // AAB?? B?
                if (!pkt_.has(pairB)) {
                    PktCards testPkt(pairB, Card::wildSuit, Card::wildCard);
                    rank += mask_.remove(testPkt);
                } else {
                    rankOneKicker(
                        rank,
                        VecCard(1, Card(pairB, Card::wildSuit)),
                        cbd_.highestValue(PktVals(pairA, pairB)),
                        AvoidPktVals(pairA, pairB)
                    );
                    return rank;
                }
                ++itB;
            }
            // Pair B is not on the board, must be pocket pairs
            PktCards testPkt(pairB, Card::wildSuit, pairB, Card::wildSuit);
            if (pkt_ == testPkt) {
                return rank;
            } else {
                rank += mask_.remove(testPkt);
            }
        }
    } else {
        // No pairs on the board, pocket must pair with singles
        for (auto itA = stats.cbegin(); itA != stats.cend(); ++itA) {
            for (auto itB = itA + 1; itB != stats.cend(); ++itB) {
                PktCards testPkt(
                    itA->value(), Card::wildSuit,
                    itB->value(), Card::wildSuit
                );
                if (pkt_ == testPkt) {
                    return rank;
                } else {
                    rank += mask_.remove(testPkt);
                }
            }
        }
    }

    // Check for pair
        // We know no two pairs exist at this point
        // If a pair is on the board:
            // Look for two values that beat top three non-paired board cards
            // Remove them: (valA valB)
            // Look for single values that beat top three non-paired board cards
            // Remove it: (valA wild)
            // exit with (wild wild)

    // Check for a pair
    {
        auto it = stats.cbegin();
        for (CardVal pairVal = Card::ace; pairVal > Card::lowAce; --pairVal) {
            if (it != stats.cend() && pairVal == it->value()) {
                if (it->nCards() == 2) {
                    // Value is paired on the board, look for two kickers
                    // PP??? ??

                    // Find two low values, no need for iterator checking
                    rankTwoKickers(
                        rank,
                        cbd_.lowestTwoValues(pairVal),
                        AvoidValue(it->value())
                    );
                    return rank;
                } else {
                    // Value is single on the board, need one in pocket
                    // P???? P?
                    if (!pkt_.has(pairVal)) {
                        PktCards testPkt(
                            pairVal, Card::wildSuit,
                            Card::wildCard
                        );
                        rank += mask_.remove(testPkt);
                    } else {
                        rankOneKicker(
                            rank,
                            VecCard(1, Card(pairVal, Card::wildSuit)),
                            cbd_.lowestValue(pairVal),
                            AvoidValue(pairVal)
                        );
                        return rank;
                    }
                }
                ++it;
                continue;
            }
            // Value is not on the board, need pocket pairs
            PktCards testPkt(pairVal, Card::wildSuit, pairVal, Card::wildSuit);
            if (testPkt == pkt_) {
                return rank;
            } else {
                rank += mask_.remove(testPkt);
            }
        }
    }


    // Check for high card
        // boardA > boardB are lowest two values on board
        // Look for two value hands that beat both first and remove them (valA valB)
        // Look for single value hands that beats boardB (valA>boardB wild)
        // exit (wild wild)

    // Check for high card
    rankTwoKickers(
        rank,
        cbd_.lowestTwoValues(),
        AvoidNone()
    );
    return rank;
}

// ****** END ****** //
