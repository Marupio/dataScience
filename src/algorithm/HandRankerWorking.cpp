#include<HandRanker.h>
#include<iostream>

// I'm adding suited operator == to card and adding wild cards to card operators
// Need to add deckMask, board, cardSet

short getRank(const Board& bd, const PktCards& pkt){
    short rank = 0;
    pocketMask mask(bd, pkt);

    // Check for straight flushes
    const Suit flushSuit = bd.flushSuit();
    const VecCardVal& flushVals(bd.flushVals());
    if (flushSuit != Card::unknownSuit) {
        const StraightCompleters straightFlushes(
            findStraightCompleters(flushVals)
        );
        for (
            auto sfit = straightFlushes.begin();
            sfit != straightFlushes.end();
            ++sfit
        ) {
            for (
                auto vit = sfit->begin();
                vit != sfit->end();
                ++vit
            ) {
                PktCards testPkt(vit->first, flushSuit, vit->second, flushSuit);
                if (testPkt == pkt) {
                    return rank;
                } else {
                    rank += mask.remove(testPkt);
                }
            }
        }
    }

    // Check for four-of-a-kind
        // If FOAK showing on board:
            // rank by cards higher than fifth card (val wild)
            // then exit (wild wild)
        // If TOAK showing on board (valA):
            // If pkt ! have valA, remove (valA, wild)
            // Otherwise:
                // Rank extra pocket card A..lowest on board (valA, val)
                // Below that (valA, wild)
        // If one or more pairs showing on board (valA) > (valB)
            // (valA valA) suits known
            // then (valB valB) suits known, if second pair is present
            // Continue
        // Check TOAK and pairs for highest value
            // TOAK - rank extra pocket card higher first, then wild
            // pairs - specific pkt only

    // Check for four-of-a-kind (FOAK)
    const VecValStats& stats(bd.stats());
    #ifdef DSDEBUG
        if (stats.size() < 3) {
            FatalError << "Card value stats array has an unexpected size: "
                << stats.size() << std::endl;
            abort();
        }
    #endif
    for (const auto it = stats.begin(); it != stats.end(); ++it) {
        switch (it->nCards()) {
        case 4: {
            // FOAK is on the board
            // rank by cards higher than fifth card (val wild)
            // then exit (wild wild)
            const CardVal foakVal = it->value();
            CardVal lowVal = stats.front().value();
            if (lowVal = foakVal) {
                lowVal = (it + 1)->value();
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
            // TOAK is on the board
            const toakCard(it->value(), bd.toakMissingSuit());
            PktCards foakPkt(toakCard, Card::wildCard);
            if (pkt == foakPkt) {
                // Player has FOAK
                const auto itR = stats.rbegin();
                CardVal lowVal = itR->value();
                if (lowVal == it->value()) {
                    itR++;
                    lowVal = itR->value();
                }
                for (CardVal kicker = Card::ace; kicker > lowVal; --kicker) {
                    if (kicker == it->value()) {
                        continue;
                    }
                    PktCards testPkt(toakCard, kicker, Card::wildSuit);
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
                            << "expected, but board pairA = " << bd.pairA
                            << ", and pairB = " << bd.pairB << std::endl;
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
        // TOAK or pair must be present
        // Only sets possible are present in bd.values
        // Start with the highest and move down
        // If it's a TOAK
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
        bd.pairA() != unknownValue
     || bd.pairB() != unknownValue
     || bd.toak() != unknownValue
    ) {
        // Full house is possible
        for (const auto it = stats.begin(); it != stats.end(); ++it) {
            // Look for highest set
            switch (it->nCards()) {
            case 3: {
                // FH set is on the board, pocket is free
                const auto itp = stats.begin();
                for (CardVal pairVal = Card::ace; pairVal > lowAce; --pairVal) {
                    if (pairVal == it->value()) {
                        // Pair value can't be set value
                        if (itp == it) {
                            ++itp;
                        }
                        continue;
                    }
                    if (itp != stats.end() && pairVal == itp->value()) {
                        // best pair is a value on the board
                        if (itp->nCards() >= 2) {
                            // best pair is already paired on the board
                            // all remaining hands the same
                            return rank;
                        } else {
                            // It is a single
                            PktCards testPocket(
                                pairVal,
                                Card::wildSuit,
                                Card::wildCard
                            );
                            if (pkt == testPocket) {
                                return rank;
                            } else {
                                rank += mask.remove(testPkt);
                            }
                        }
                        ++itp;
                    } else {
                        // Not on the board, must be pocket paired
                        PktCards testPocket(
                            pairVal,
                            Card::wildSuit,
                            pairVal,
                            Card::wildSuit
                        );
                        if (pkt == testPocket) {
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
                for (
                    const auto itp = stats.begin();
                    itp != stats.end();
                    ++itp
                ) {
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
                        if (pkt == testPocket) {
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
                        if (pkt == testPocket) {
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
                for (
                    const auto itp = stats.begin();
                    itp != stats.end();
                    ++itp
                ) {
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
                        if (pkt == testPocket) {
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
    switch (flushVals.size()) {
    case 5: {
        const auto itR = flushVals.rbegin();
        CardVal lowValA = *itR;
        itR++;
        CardVal lowValB = *itR;
        // Look for two kickers
        const auto itH = flushVals.begin();
        for (
            CardVal highKicker = Card::ace;
            highKicker > lowValB;
            ++highKicker
        ) {
            if (itH != flushVals.end() && highKicker == *itH) {
                ++itH;
                continue;
            }
            const auto itH = flushVals.begin();
            for (
                CardVal lowKicker = Card::king;
                highKicker > lowValA;
                ++highKicker
            ) {
                if (itL != flushVals.end() && lowKicker == *itL) {
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
        itH = flushVals.begin();
        for (
            CardVal kicker = Card::ace;
            kicker > lowValA;
            --kicker
        ) {
            if (itH != flushVals.end() && kicker == *itH) {
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
            const auto itH = flushVals.begin();
            for (
                CardVal kicker = Card::ace;
                kicker > Card::two;
                --kicker
            ) {
                if (itH != flushVals.end() && kicker == *itH) {
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
        } else {
            PktVals testPkt(Card::wildValue, flushSuit, Card::wildCard);
            rank += mask.remove(testPkt);
        }
        break;
    }
    case 3: {
        if (pkt.suited(flushSuit)) {
            const auto itH = flushVals.begin();
            for (
                CardVal highKicker = Card::ace;
                highKicker > Card::three;
                ++highKicker
            ) {
                if (itH != flushVals.end() && highKicker == *itH) {
                    ++itH;
                    continue;
                }
                const auto itL = flushVals.begin();
                for (
                    CardVal lowKicker = Card::king;
                    highKicker > Card::two;
                    ++highKicker
                ) {
                    if (itL != flushVals.end() && lowKicker == *itL) {
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
        } else {
            PktVals testPkt(
                Card::wildValue,
                flushSuit,
                Card::wildValue,
                flushSuit
            );
            rank += mask.remove(testPkt);
        }
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
        VecCardVal values;
        values.reserve(stats.size());
        for (const auto it = stats.begin(); it != stats.end(); ++it) {
            values.push_back(it->value());
        }
        const StraightCompleters straights(findStraightCompleters(flushVals));
        for (
            auto it = straights.begin();
            it != straights.end();
            ++it
        ) {
            for (
                auto vit = it->begin();
                vit != it->end();
                ++vit
            ) {
                PktCards testPkt(
                    vit->first,
                    Card::wildSuit,
                    vit->second,
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
    
    // Check for sets
        // Run through all bd.value
            // If exists as TOAK:
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

    // Check for three-of-a-kind (TOAK)
    for (const auto it = stats.begin(); it != stats.end(); ++it) {
        switch (it->nCards()) {
        case 3: {
            // TOAK is on board, look for two kickers
            const auto itR = stats.rbegin();
            CardVal lowValA = itR->value();
            CardVal lowValB;
            if (lowValA == it->value()) {
                itR++;
                lowValA = itR->value();
                itR++;
                lowValB = itR->value();
            } else {
                itR++;
                lowValB = itR->value();
                if (lowValB == it->value()) {
                    itR++;
                    lowValB = itR->value();
                }
            }
            const auto itA = stats.begin();
            for (CardVal kickerA = Card::ace; kickerA > lowValB; --kickerA) {
                if (itA != stats.end() && kickerA == itA->value()) {
                    ++itA;
                    continue;
                }
                const auto itB = stats.begin();
                for (
                    CardVal kickerB = Card::king;
                    kickerB > lowValA;
                    --kickerB
                ) {
                    if (itB != stats.end() && kickerB == itB->value()) {
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
            itA = stats.begin();
            for (CardVal kicker = Card::ace; kicker > lowValA; --kicker) {
                if (itA != stats.end() && kicker == itA->value()) {
                    ++itA;
                    continue;
                }
                PktCards testPkt(kickerA, Card::wildSuit, Card::wildCard);
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
            // TOAK uses a pair on the board, one pocket card is free
            const PktSuits& missingSuits (
                it->value() == bd.pairA()
              ? bd.pairAMissingSuits()
              : bd.pairBMissingSuits()
            );
            
            // We could use wildSuit, but we know what suits are missing, so
            // we specify the suits and check two cards to reduce effort
            Card completerA(it->value(), missingSuits.first);
            Card completerB(it->value(), missingSuits.second);
            const auto itR = stats.rbegin();
            CardVal lowVal = itR->value();
            if (lowVal == it->value()) {
                itR++;
                lowVal = itR->value();
            }
            const auto itA = stats.begin();
            for (CardVal kicker = Card::ace; kicker > lowVal; --kicker) {
                if (itA != stats.end() && kicker == itA->value) {
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
            break;
        }
        case 1: {
            // TOAK uses a single on the board, pocket pairs required
            PktCards testPkt (
                it->value,
                Card::wildSuit,
                it->value,
                Card::wildSuit
            );
            if (pkt == testPkt) {
                return rank;
            } else {
                rank += make.remove(testPkt);
            }
            break;
        }
        default {
            FatalError << "Unexpected nCards when checking for TOAK. nCards "
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
    for (const auto it = stats.begin(); it != stats.end(); ++it) {
        switch (it->nCards()) {
        case 2: {
            // One pair is on the board. Second pair can be pocket pairs or
            // paired with another single
            const CardVal pairA = it->value();
            const auto itA = stats.begin();
            for (CardVal pairB = Card::ace; pairB > Card::lowAce; --pairB) {
                if (pairB == pairA) {
                    ++itA;
                    continue;
                }
                if (itA != stats.end() && pairB == itA->value()) {
                    // Paired with another single, search for kicker
                    const auto itR = stats.rbegin();
                    CardVal lowVal = itR->value();
                    if (lowVal == it->value()) {
                        itR++;
                        lowVal = itR->value();
                    }
                    const auto itB = stats.begin();
                    for (
                        CardVal kicker = Card::ace;
                        kicker > lowVal;
                        --kicker
                    ) {
                        if (kicker == pairA || kicker == pairB) {
                            continue;
                        }
                        if (itB != stats.end() && kicker == itB->value()) {
                            ++itB;
                            continue;
                        }
                        PktCards testPkt(
                            pairB,
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
                    ++itA;
                } else {
                    // Value not on board, pocket pairs required
                    PktCards testPkt(
                        pairB,
                        Card::wildSuit,
                        pairB,
                        Card::wildSuit
                    );
                    if (testPkt == pkt) {
                        return rank;
                    } else {
                        rank += mask.remove(testPkt);
                    }
                }
            }
            break;
        }
        case 1: {
            // Pair A uses single from board
            Card pairACardA;
            Card pairACardB;
            if (it->value() == bd.pairA()) {
                pairACardA = Card(it->value, bd.pairAMissingSuits().first);
                pairACardB = Card(it->value, bd.pairAMissingSuits().second);
            } else {
                pairACardA = Card(it->value, bd.pairBMissingSuits().first);
                pairACardB = Card(it->value, bd.pairBMissingSuits().second);
            }
            for (const auto itB = stats.begin(); itB != stats.end(); ++itB) {
                if (itB.nCards() > 1) {
                    // Pair B is on the board, look for a kicker
                    const auto itR = stats.rbegin();
                    CardVal lowVal = itR->value();
                    if (lowVal == it->value()) {
                        itR++;
                        lowVal = itR->value();
                    }
                    const auto itK = stats.begin();
                    for (CardVal kicker = Card::ace; kicker > lowVal) {
                        if (itK != stats.end() && kicker == *itK) {
                            ++itK;
                            continue;
                        }
                        if (kicker == pairA || kicker == pairB) {
                            continue;
                        }
                        PktCards testPktA(pairACardA, kicker, Card::wildSuit);
                        PktCards testPktB(pairACardB, kicker, Card::wildSuit);
                        if (pkt == testPktA || pkt == testPktB) {
                            return rank;
                        } else {
                            rank += mask.remove(testPktA);
                            rank += mask.remove(testPktB);
                        }
                    }
                } else {
                    // Pair B must be in pocket, and must be bd.pairB
                    const Card pairBCardA(
                        itB->value,
                        bd.pairBMissingSuits().first
                    );
                    const Card pairBCardA(
                        itB->value,
                        bd.pairBMissingSuits().second
                    );
                    PktCards testPktAA(pairACardA, pairBCardA);
                    PktCards testPktAB(pairACardA, pairBCardB);
                    PktCards testPktBA(pairACardB, pairBCardA);
                    PktCards testPktBB(pairACardB, pairBCardB);
                    if (
                        pkt == testPktAA
                     || pkt == testPktAB
                     || pkt == testPktBA
                     || pkt == testPktBB
                    ) {
                        return rank;
                    } else {
                        rank += mask.remove(testPktAA);
                        rank += mask.remove(testPktAB);
                        rank += mask.remove(testPktBA);
                        rank += mask.remove(testPktBB);
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
    // Check for pair
        // We know no two pairs exist at this point
        // If a pair is on the board:
            // Look for two values that beat top three non-paired board cards
            // Remove them: (valA valB)
            // Look for single values that beat top three non-paired board cards
            // Remove it: (valA wild)
            // exit with (wild wild)
    // Check for high card
        // boardA > boardB are lowest two values on board
        // Look for two value hands that beat both first and remove them (valA valB)
        // Look for single value hands that beats boardB (valA>boardB wild)
        // exit (wild wild)

}

