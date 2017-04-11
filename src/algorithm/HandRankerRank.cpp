#include<PktMask.h>

// ****** Public Member Functions ****** //

short ds::HandRanker::getRank(const Board& bd, const PktCards& pkt){
    short rank = 0;
    PktMask mask(bd, pkt);

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
        if (stats.nCards() < 3) {
            FatalError << "Card value stats array has an unexpected size: "
                << stats.nCards() << std::endl;
            abort();
        }
    #endif
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
            // TOAK is on the board
            const Card toakCard(it->value(), bd.toakMissingSuit());
            PktCards foakPkt(toakCard, Card::wildCard);
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
    switch (flushVals.size()) {
    case 5: {
        auto itR = flushVals.crbegin();
        CardVal lowValA = *itR;
        itR++;
        CardVal lowValB = *itR;
        // Look for two kickers
        auto itH = flushVals.cbegin();
        for (
            CardVal highKicker = Card::ace;
            highKicker > lowValB;
            ++highKicker
        ) {
            if (itH != flushVals.cend() && highKicker == *itH) {
                ++itH;
                continue;
            }
            auto itL = flushVals.cbegin();
            for (
                CardVal lowKicker = Card::king;
                highKicker > lowValA;
                ++highKicker
            ) {
                if (itL != flushVals.cend() && lowKicker == *itL) {
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
        itH = flushVals.cbegin();
        for (
            CardVal kicker = Card::ace;
            kicker > lowValA;
            --kicker
        ) {
            if (itH != flushVals.cend() && kicker == *itH) {
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
            auto itH = flushVals.cbegin();
            for (
                CardVal kicker = Card::ace;
                kicker > Card::two;
                --kicker
            ) {
                if (itH != flushVals.cend() && kicker == *itH) {
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
            auto itH = flushVals.cbegin();
            for (
                CardVal highKicker = Card::ace;
                highKicker > Card::three;
                ++highKicker
            ) {
                if (itH != flushVals.cend() && highKicker == *itH) {
                    ++itH;
                    continue;
                }
                auto itL = flushVals.cbegin();
                for (
                    CardVal lowKicker = Card::king;
                    highKicker > Card::two;
                    ++highKicker
                ) {
                    if (itL != flushVals.cend() && lowKicker == *itL) {
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
                return rank;
            } else {
                rank += mask.remove(testPkt);
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
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 3: {
            // TOAK is on board, look for two kickers

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
            // TOAK uses a single on the board, pocket pairs required
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
    if (bd.pairA != Card::unknownValue) {
        // Pocket pair could be highest, have to iterate through all cards
        const CardVal lowVal = stats.back().value();

        auto itP = stats.cbegin();

        for (CardVal pairA = Card::ace; pairA > lowVal; --pairA) {
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
                            // PairB is on the board
                            if (itPB->nCards() == 2) {
                                // PairB is paired on the board, look for
                                // kicker from both pocket cards
                                &&&
                            } else {
                                // PairB is single on the board, look for
                                // kicker from one pocket card
                                &&&
                            }
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
        for (auto itA = stats.cbegin(); it != stats.cend(); ++it)
&&&&
    }
    for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
        switch (it->nCards()) {
        case 2: {
            // One pair is on the board. Second pair can be pocket pairs or
            // paired with another single
            const CardVal pairA = it->value();
            auto itA = stats.cbegin();
            for (CardVal pairB = Card::ace; pairB > Card::lowAce; --pairB) {
                if (pairB == pairA) {
                    ++itA;
                    continue;
                }
                if (itA != stats.cend() && pairB == itA->value()) {
                    // Paired with another single, search for kicker

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

                    auto itB = stats.cbegin();
                    for (
                        CardVal kicker = Card::ace;
                        kicker > lowVal;
                        --kicker
                    ) {
                        if (kicker == pairA || kicker == pairB) {
                            continue;
                        }
                        if (itB != stats.cend() && kicker == itB->value()) {
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
            const CardVal pairA = it->value();
            if (it->value() == bd.pairA()) {
                pairACardA = Card(it->value(), bd.pairAMissingSuits().first);
                pairACardB = Card(it->value(), bd.pairAMissingSuits().second);
            } else {
                pairACardA = Card(it->value(), bd.pairBMissingSuits().first);
                pairACardB = Card(it->value(), bd.pairBMissingSuits().second);
            }
            for (auto itB = stats.cbegin(); itB != stats.cend(); ++itB) {
                if (itB->nCards() > 1) {
                    // Pair B is on the board, look for a kicker
                    const CardVal pairB = itB->value();

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

                    auto itK = stats.cbegin();
                    for (
                        CardVal kicker = Card::ace;
                        kicker > lowVal;
                        --kicker
                    ) {
                        if (itK != stats.cend() && kicker == itK->value()) {
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
                    short rankOffset = 0;
                    for (
                        CardVal nonKicker = lowVal;
                        nonKicker >= Card::two;
                        --nonKicker
                    ) {
                        PktCards testPktA(pairACardA, Card::wildCard);
                        PktCards testPktB(pairACardB, Card::wildCard);
                        if (pkt == testPktA || pkt == testPktB) {
                            return rank;
                        } else {
                            rankOffset += mask.remove(testPktA);
                            rankOffset += mask.remove(testPktB);
                        }
                    }
                    rank += rankOffset;
                } else {
                    // Pair B must be in pocket, and must be bd.pairB
                    const Card pairBCardA(
                        itB->value(),
                        bd.pairBMissingSuits().first
                    );
                    const Card pairBCardB(
                        itB->value(),
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

    // Check for a pair
    {
        auto it = stats.cbegin();
        for (CardVal pairVal = Card::ace; pairVal > Card::lowAce; --pairVal) {
            if (it != stats.cend() && pairVal == it->value()) {
                // Pair card is on the board, look for kicker
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
