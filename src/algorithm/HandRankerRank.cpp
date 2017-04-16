#include<PktMask.h>

// ****** Public Member Functions ****** //

short ds::HandRanker::getRank(const Board& bd, const PktCards& pkt){
    short rank = 0;
    PktMask mask(bd, pkt);

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
                sfit->second.first,
                flushSuit,
                sfit->second.second,
                flushSuit
            );
            if (testPkt == pkt) {
                return rank;
            } else {
                rank += mask.remove(testPkt);
            }
        }
    }

    // Check for four-of-a-kind
        // If FOAK showing on board:
            // rank by cards higher than fifth card (val wild)
            // then exit (wild wild)
        // If set showing on board (valA):
            // If pkt ! have valA, remove (valA, wild)
            // Otherwise:
                // Rank extra pocket card A..lowest on board (valA, val)
                // Below that (valA, wild)
        // If one or more pairs showing on board (valA) > (valB)
            // (valA valA) suits known
            // then (valB valB) suits known, if second pair is present
            // Continue
        // Check set and pairs for highest value
            // set - rank extra pocket card higher first, then wild
            // pairs - specific pkt only

    // Check for four-of-a-kind (FOAK)
    const VecValStats& stats(bd.stats());
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 4: {
            // FOAK is on the board
            // rank by cards higher than fifth card (val wild)
            // then exit (wild wild)

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

            for (CardVal kicker = Card::ace; kicker > lowVal; --kicker) {
                PktCards testPkt(kicker, Card::wildSuit, Card::wildCard);
                if (testPkt == pkt) {
                    return rank;
                } else {
                    rank += mask.remove(testPkt);
                }
            }
            // else all remaining hands rank the same
            return rank;
            break;
        }
        case 3: {
            // Set is on the board
            const Card setCard(it->value(), bd.setMissingSuit());
            PktCards foakPkt(setCard, Card::wildCard);
            if (pkt == foakPkt) {
                // Player has FOAK
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
                for (CardVal kicker = Card::ace; kicker > lowVal; --kicker) {
                    if (kicker == it->value()) {
                        continue;
                    }
                    PktCards testPkt(setCard, kicker, Card::wildSuit);
                    if (testPkt == pkt) {
                        return rank;
                    } else {
                        rank += mask.remove(testPkt);
                    }
                }
                // Since pkt == foakPkt
                return rank;
            } else {
                // Player does not have FOAK
                rank += mask.remove(foakPkt);
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
                return rank;
            } else {
                rank += mask.remove(foakPkt);
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
        // Only sets possible are present in bd.values
        // Start with the highest and move down
        // If it's a set
            // Run through all other cards for pair:
                // (val val) if not present on board
                // (val wild) if present as a single
                // Exit with (wild wild) if present as a pair
        // If two pairs, then start with highest (call it valA)
            // Run through all other bd.values as (valA, val)
            // Once you reach the other pair, it's (valA, wild)
            // Then repeat with (valB)
        // If one pair
            // Run through bd.values for set first (valA)
                // If it is the pair:
                    // then run through bd.values for pair (valA val)
                // If it is single:
                    // pocket pairs only (valB valB)
        // Continue

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
                        if (itp->nCards() >= 2) {
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
                            if (pkt == testPkt) {
                                return rank;
                            } else {
                                rank += mask.remove(testPkt);
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
                        if (pkt == testPkt) {
                            return rank;
                        } else {
                            rank += mask.remove(testPkt);
                        }
                    }
                }
                break;
            }
            case 2: {
                // FH set uses a pair on the board, needs one card in hand
                // setVal = it->value
                for (auto itp = stats.cbegin(); itp != stats.cend(); ++itp) {
                    if (itp == it) {
                        continue;
                    }
                    if (itp->nCards() >= 2) {
                        // Pair is on the board
                        PktCards testPkt(
                            it->value(),
                            Card::wildSuit,
                            Card::wildCard
                        );
                        if (pkt == testPkt) {
                            return rank;
                        } else {
                            rank += mask.remove(testPkt);
                        }
                    } else {
                        // FH pair uses single from the board
                        PktCards testPkt(
                            it->value(),
                            Card::wildSuit,
                            itp->value(),
                            Card::wildSuit
                        );
                        if (pkt == testPkt) {
                            return rank;
                        } else {
                            rank += mask.remove(testPkt);
                        }
                    }
                }
                break;
            }
            case 1: {
                // single is the set, needs pocket pairs and pair on the board                
                for (auto itp = stats.cbegin(); itp != stats.cend(); ++itp) {
                    if (itp == it) {
                        continue;
                    }
                    if (itp->nCards() >= 2) {
                        PktCards testPkt(
                            it->value(),
                            Card::wildSuit,
                            it->value(),
                            Card::wildSuit
                        );
                        if (pkt == testPkt) {
                            return rank;
                        } else {
                            rank += mask.remove(testPkt);
                        }
                    }
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
        if (!pkt.has(flushSuit)) {
            PktCards testPkt(
                Card::wildValue,
                flushSuit,
                Card::wildValue,
                flushSuit
            );
            rank += mask.remove(testPkt);
            return rank;
        }
        auto itR = flushValues.crbegin();
        CardVal lowValA = *itR;
        itR++;
        CardVal lowValB = *itR;
        // Look for two kickers
        auto itH = flushValues.cbegin();
        for (
            CardVal highKicker = Card::ace;
            highKicker > lowValB;
            ++highKicker
        ) {
            if (itH != flushValues.cend() && highKicker == *itH) {
                ++itH;
                continue;
            }
            auto itL = flushValues.cbegin();
            if (*itL == Card::ace) {
                ++itL;
            }
            for (
                CardVal lowKicker = Card::king;
                highKicker > lowValA;
                ++highKicker
            ) {
                if (itL != flushValues.cend() && lowKicker == *itL) {
                    ++itL;
                    continue;
                }
                PktCards testPkt(highKicker, flushSuit, lowKicker, flushSuit);
                if (pkt == testPkt) {
                    return rank;
                } else {
                    rank += mask.remove(testPkt);
                }
            }
        }
        // Look for one kicker
        itH = flushValues.cbegin();
        for (
            CardVal kicker = Card::ace;
            kicker > lowValA;
            --kicker
        ) {
            if (itH != flushValues.cend() && kicker == *itH) {
                ++itH;
                continue;
            }
            PktCards testPkt(kicker, flushSuit, Card::wildCard);
            if (pkt == testPkt) {
                return rank;
            } else {
                rank += mask.remove(testPkt);
            }
        }
        // all other hands the same
        return rank;
        break;
    }
    case 4: {
        if (pkt.has(flushSuit)) {
            // Sort by pocket card that completes the flush
            auto itH = flushValues.cbegin();
            for (
                CardVal kicker = Card::ace;
                kicker > Card::two;
                --kicker
            ) {
                if (itH != flushValues.cend() && kicker == *itH) {
                    ++itH;
                    continue;
                }
                PktCards testPkt(kicker, flushSuit, Card::wildCard);
                if (pkt == testPkt) {
                    return rank;
                } else {
                    rank += mask.remove(testPkt);
                }
            }
            FatalError << "Rank should have been returned for flush."
                << std::endl;
            abort();
        } else {
            PktCards testPkt(Card::wildValue, flushSuit, Card::wildCard);
            rank += mask.remove(testPkt);
        }
        break;
    }
    case 3: {
        if (pkt.suited(flushSuit)) {
            auto itH = flushValues.cbegin();
            for (
                CardVal highKicker = Card::ace;
                highKicker > Card::three;
                ++highKicker
            ) {
                if (itH != flushValues.cend() && highKicker == *itH) {
                    ++itH;
                    continue;
                }
                auto itL = flushValues.cbegin();
                if (*itL == Card::ace) {
                    ++itL;
                }
                for (
                    CardVal lowKicker = Card::king;
                    highKicker > Card::two;
                    ++highKicker
                ) {
                    if (itL != flushValues.cend() && lowKicker == *itL) {
                        ++itL;
                        continue;
                    }
                    PktCards testPkt(
                        highKicker,
                        flushSuit,
                        lowKicker,
                        flushSuit
                    );
                    if (pkt == testPkt) {
                        return rank;
                    } else {
                        rank += mask.remove(testPkt);
                    }
                }
            }
            FatalError << "Rank should have been returned for flush."
                << std::endl;
            abort();
        } else {
            PktCards testPkt(
                Card::wildValue,
                flushSuit,
                Card::wildValue,
                flushSuit
            );
            rank += mask.remove(testPkt);
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
                it->second.first,
                Card::wildSuit,
                it->second.second,
                Card::wildSuit
            );
            if (testPkt == pkt) {
                return rank;
            } else {
                rank += mask.remove(testPkt);
            }
        }
    }
    
    // Check for sets
        // Run through all bd.value
            // If exists as set:
                // Run through (valA, valB) higher than lowest two on board
                // Then through (valA wild) higher than lowest on board
                // Then exit (wild wild)
            // If exists as pair (valA)
                // If pkt ! have valA, remove (valA, wild)
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

            // Find two low values
            auto itR = stats.crbegin();
            CardVal lowValA = Card::lowAce;
            CardVal lowValB = Card::lowAce;
            while (
                itR != stats.crend()
             && (lowValA == it->value() || lowValA == Card::lowAce)
            ) {
                lowValA = itR->value();
                ++itR;
            }
            while (
                itR != stats.crend()
             && (lowValB == it->value() || lowValB == Card::lowAce)
            ) {
                lowValB = itR->value();
                ++itR;
            }
            
            auto itA = stats.cbegin();
            for (CardVal kickerA = Card::ace; kickerA > lowValB; --kickerA) {
                if (itA != stats.cend() && kickerA == itA->value()) {
                    ++itA;
                    continue;
                }
                if (kickerA == it->value()) {
                    continue;
                }
                auto itB = stats.cbegin();
                if (itB->value() == Card::ace) {
                    ++itB;
                }
                for (
                    CardVal kickerB = Card::king;
                    kickerB > lowValA;
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
                    if (pkt == testPkt) {
                        return rank;
                    } else {
                        rank += mask.remove(testPkt);
                    }
                }
            }
            // Didn't find two kickers, look for one kicker
            itA = stats.cbegin();
            for (CardVal kicker = Card::ace; kicker > lowValA; --kicker) {
                if (itA != stats.cend() && kicker == itA->value()) {
                    ++itA;
                    continue;
                }
                PktCards testPkt(kicker, Card::wildSuit, Card::wildCard);
                if (pkt == testPkt) {
                    return rank;
                } else {
                    rank += mask.remove(testPkt);
                }
            }
            // Didn't find one kicker, all remaining hands the same
            return rank;
            break;
        }
        case 2: {
            // set uses a pair on the board, one pocket card is free
            const PktSuits& missingSuits (
                it->value() == bd.pairA()
              ? bd.pairAMissingSuits()
              : bd.pairBMissingSuits()
            );
            
            // We could use wildSuit, but we know what suits are missing, so
            // we specify the suits and check two cards to reduce effort
            Card completerA(it->value(), missingSuits.first);
            Card completerB(it->value(), missingSuits.second);
            
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

                auto itA = stats.cbegin();
                for (CardVal kicker = Card::ace; kicker > lowVal; --kicker) {
                    if (itA != stats.cend() && kicker == itA->value()) {
                        ++itA;
                        continue;
                    }
                    PktCards testPktA(completerA, kicker, Card::wildSuit);
                    PktCards testPktB(completerB, kicker, Card::wildSuit);
                    if (pkt == testPktA || pkt == testPktB) {
                        return rank;
                    } else {
                        rank += mask.remove(testPktA);
                        rank += mask.remove(testPktB);
                    }
                }
            } else {
                PktCards testPktA(completerA, Card::wildCard);
                PktCards testPktB(completerB, Card::wildCard);
                rank += mask.remove(testPktA);
                rank += mask.remove(testPktB);
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
            if (pkt == testPkt) {
                return rank;
            } else {
                rank += mask.remove(testPkt);
            }
            break;
        }
        default: {
            FatalError << "Unexpected nCards when checking for set. nCards "
                << "is: " << it->nCards() << std::endl;
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
    if (bd.pairA() != Card::unknownValue) {
        // Pocket pair could be highest, have to iterate through all cards
        const CardVal low = stats.back().value();

        auto itP = stats.cbegin();

        for (CardVal pairA = Card::ace; pairA > low; --pairA) {
            if (pairA == itP->value()) {
                if (itP->nCards() == 2) {
                    // PairA is paired on the board, next pair can be pocket
                    // pairs
                    auto itPB = itP + 1;
                    for (
                        CardVal pairB = pairA - 1;
                        pairB > Card::lowAce;
                        --pairB
                    ) {
                        if (pairB == itPB->value()) {
                            // PairB is on the board, need to look for kicker

                            // Find one low value
                            auto itR = stats.crbegin();
                            CardVal lowVal = itR->value();
                            if (lowVal == pairA || lowVal == pairB) {
                                itR++;
                                if (itR != stats.crend()) {
                                    lowVal = itR->value();
                                } else {
                                    lowVal = Card::lowAce;
                                }
                            }

                            CardVal pairBVal = Card::wildValue;
                            if (itPB->nCards() == 2) {
                                // PairB is paired on the board, look for
                                // one kicker from both pocket cards
                                // i.e. pairBVal is wild, do nothing
                            } else {
                                // PairB is single on the board, look for
                                // one kicker from one pocket card
                                // i.e. pairBVal is pairValue, set it
                                pairBVal = pairB;
                            }
                            for (
                                CardVal kicker = Card::ace;
                                kicker > lowVal;
                                --kicker
                            ) {
                                if (kicker == pairA || kicker == pairB) {
                                    continue;
                                }
                                PktCards testPkt(
                                    kicker,
                                    Card::wildSuit,
                                    pairBVal,
                                    Card::wildSuit
                                );
                                if (pkt == testPkt) {
                                    return rank;
                                } else {
                                    rank += mask.remove(testPkt);
                                }
                            }
                            short rankOffset = 0;
                            for (
                                CardVal kicker = lowVal - 1;
                                kicker > Card::lowAce;
                                --kicker
                            ) {
                                if (kicker == pairA || kicker == pairB) {
                                    continue;
                                }
                                PktCards testPkt(
                                    kicker,
                                    Card::wildSuit,
                                    pairBVal,
                                    Card::wildSuit
                                );
                                if (pkt == testPkt) {
                                    return rank;
                                } else {
                                    rankOffset += mask.remove(testPkt);
                                }
                            }
                            rank += rankOffset;
                            ++itPB;
                        }
                    }
                } else {
                    // PairA is single on the board
                    for (auto itB = itP + 1; itB != stats.cend(); ++itB) {
                        PktCards testPkt(
                            pairA,
                            Card::wildSuit,
                            itB->value(),
                            Card::wildSuit
                        );
                        if (testPkt == pkt) {
                            return rank;
                        } else {
                            rank += mask.remove(testPkt);
                        }
                    }
                }
                ++itP;
            } else {
                // PairA is not on the board, must be pocket pairs
                PktCards testPkt(
                    pairA,
                    Card::wildSuit,
                    pairA,
                    Card::wildSuit
                );
                if (testPkt == pkt) {
                    return rank;
                } else {
                    rank += mask.remove(testPkt);
                }
            }
        }
    } else {
        // Must pair with two board cards, can iterate through stats
        for (auto itA = stats.cbegin(); itA != stats.cend(); ++itA) {
            for (auto itB = itA + 1; itB != stats.cend(); ++itB) {
                PktCards testPkt(
                    itA->value(),
                    Card::wildSuit,
                    itB->value(),
                    Card::wildSuit
                );
                if (pkt == testPkt) {
                    return rank;
                } else {
                    rank += mask.remove(testPkt);
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

                    // Find two low values, no need for iterator checking
                    auto itR = stats.crbegin();
                    CardVal lowValA = itR->value();
                    CardVal lowValB;
                    if (lowValA == pairVal) {
                        ++itR;
                        lowValA = itR->value();
                        ++itR;
                        lowValB = itR->value();
                    } else {
                        ++itR;
                        lowValB = itR->value();
                        if (lowValB == pairVal) {
                            ++itR;
                            lowValB = itR->value();
                        }
                    }
                    
                    auto itO = stats.cbegin();
                    for (
                        CardVal highKicker = Card::ace;
                        highKicker < lowValB;
                        --highKicker
                    ) {
                        if (itO != stats.cend() && highKicker == itO->value()) {
                            ++itO;
                            continue;
                        }
                        auto itI = stats.cbegin();
                        if (itI->value() == Card::ace) {
                            ++itI;
                        }
                        for (
                            CardVal lowKicker = Card::king;
                            lowKicker < lowValA;
                            --lowKicker
                        ) {
                            if (
                                itI != stats.cend()
                             && lowKicker == itI->value()
                            ) {
                                ++itI;
                                continue;
                            }
                            PktCards testPkt(
                                highKicker,
                                Card::wildSuit,
                                lowKicker,
                                Card::wildSuit
                            );
                            if (pkt == testPkt) {
                                return rank;
                            } else {
                                rank += mask.remove(testPkt);
                            }
                        }

                        // Low kicker doesn't matter, continue through
                        // remaining cards
                        short rankOffset = 0;
                        for (
                            CardVal lowKicker = Card::king;
                            lowKicker < lowValA;
                            --lowKicker
                        ) {
                            if (
                                itI != stats.cend()
                             && lowKicker == itI->value()
                            ) {
                                ++itI;
                                continue;
                            }
                            PktCards testPkt(
                                highKicker,
                                Card::wildSuit,
                                lowKicker,
                                Card::wildSuit
                            );
                            if (pkt == testPkt) {
                                return rank;
                            } else {
                                rankOffset += mask.remove(testPkt);
                            }
                        }
                        rank += rankOffset;
                    }
                } else {
                    // Pair card is on the board, look for kicker
                    
                    // Find one low value, no need for iterator checking
                    auto itR = stats.crbegin();
                    CardVal lowVal = itR->value();
                    if (lowVal == pairVal) {
                        itR++;
                        lowVal = itR->value();
                    }
                    auto itK = stats.cbegin();
                    for (CardVal kicker = Card::ace; kicker > lowVal; --kicker) {
                        if (itK != stats.cend() && kicker == itK->value()) {
                            ++itK;
                            continue;
                        }
                        if (kicker == pairVal) {
                            continue;
                        }
                        PktCards testPkt(
                            pairVal,
                            Card::wildSuit,
                            kicker,
                            Card::wildSuit
                        );
                        if (testPkt == pkt) {
                            return rank;
                        } else {
                            rank += mask.remove(testPkt);
                        }
                    }
                    // No kicker
                    short rankOffset = 0;
                    for (
                        CardVal nonKicker = lowVal - 1;
                        nonKicker >= Card::two;
                        --nonKicker
                    ) {
                        PktCards testPkt(
                            pairVal,
                            Card::wildSuit,
                            nonKicker,
                            Card::wildSuit
                        );
                        if (testPkt == pkt) {
                            return rank;
                        } else {
                            rankOffset += mask.remove(testPkt);
                        }
                    }
                    rank += rankOffset;
                    ++it;
                }
            } else {
                // Pair val is not on the board, must be pocket pairs
                PktCards testPkt(
                    pairVal,
                    Card::wildSuit,
                    pairVal,
                    Card::wildSuit
                );
                if (testPkt == pkt) {
                    return rank;
                } else {
                    rank += mask.remove(testPkt);
                }
            }
        }
    }

    // Check for high card
        // boardA > boardB are lowest two values on board
        // Look for two value hands that beat both first and remove them (valA valB)
        // Look for single value hands that beats boardB (valA>boardB wild)
        // exit (wild wild)

    // Check for high card
    {
        // First check for two kickers
        
        // Find two low values - no iterator checking necessary
        auto itR = stats.crbegin();
        const CardVal lowValA = itR->value();
        ++itR;
        const CardVal lowValB = itR->value();
        auto itA = stats.cbegin();
        for (CardVal kickerA = Card::ace; kickerA > lowValB; --kickerA) {
            if (itA != stats.cend() && kickerA == itA->value()) {
                ++itA;
                continue;
            }
            auto itB = stats.cbegin();
            for (
                CardVal kickerB = Card::king;
                kickerB > lowValA;
                --kickerB
            ) {
                if (itB != stats.cend() && kickerB == itB->value()) {
                    ++itB;
                    continue;
                }
                PktCards testPkt(
                    kickerA,
                    Card::wildSuit,
                    kickerB,
                    Card::wildSuit
                );
                if (pkt == testPkt) {
                    return rank;
                } else {
                    rank += mask.remove(testPkt);
                }
            }
        }
        // Didn't find two kickers, look for one kicker
        itA = stats.cbegin();
        for (CardVal kicker = Card::ace; kicker > lowValA; --kicker) {
            if (itA != stats.cend() && kicker == itA->value()) {
                ++itA;
                continue;
            }
            PktCards testPkt(kicker, Card::wildSuit, Card::wildCard);
            if (pkt == testPkt) {
                return rank;
            } else {
                rank += mask.remove(testPkt);
            }
        }
        // Didn't find one kicker, all remaining hands the same
        return rank;
    }
}

// ****** END ****** //
