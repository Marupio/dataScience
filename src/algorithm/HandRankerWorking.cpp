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
            StraightCompleters::const_iterator sfit = straightFlushes.begin();
            sfit != straightFlushes.end();
            ++sfit
        ) {
            for (
                VecPktVals::const_iterator vit = sfit->begin();
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
    for (const auto it = stats.begin(); it != stats.end(); ++it) {
        switch(it->nCards()) {
        case 4: {
            // FOAK is on the board
            // rank by cards higher than fifth card (val wild)
            // then exit (wild wild)
            const CardVal foakVal = it->value();
            CardVal lowKicker = stats.front().value();
            if (lowKicker = foakVal) {
                lowKicker = (it + 1)->value();
            }
            for (CardVal kicker = Card::ace; kicker > lowKicker; --kicker) {
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
                CardVal lowKicker = stats.front().value();
                if (lowKicker = toakVal) {
                    lowKicker = (it + 1)->value();
                }
                for (CardVal kicker = Card::ace; kicker > lowKicker; --kicker) {
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
            switch(it->nCards()) {
            case 3: {
                // FH set is on the board, pocket is free
                const auto itp = stats.begin();
                for (CardVal pairVal = Card::ace; pairVal > lowAce; --pairVal) {
                    if (pairVal == it->value()) {
                        // Pair value can't be set value
                        ++itp;
                        continue;
                    }
                    if (pairVal == itp->value()) {
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
                for (const auto itp = stats.begin(); itp != stats.end(); ++it) {
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
                for (const auto itp = stats.begin(); itp != stats.end(); ++it) {
                    if (itp == it) {
                        continue;
                    }
                    switch (itp->nCards()) {
                    case 3: // fall through
                    case 2: {
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
                    default: {
                        break;
                    } // end default
                    } // end switch
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
    // Check for straight
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

    const valueSet& flushValues(board.sortedFlushValues());
    const card::suitEnum flushSuit(board.flushSuit());
    if (flushValues.size()) {
        straightCompleters sc(flushValues);
        for (
            straightCompleters::const_iter scIter = sc.begin();
            scIter != sc.end();
            ++scIter
        ) {
            const pocketValues& sfpv(scIter->second);
            cardSet sfPocket(sfpv, flushSuit);
            if (sfPocket == pocket) {
                // This will also match (0, 0)
                return rank;
            }
            if (sfpv.second = 0) {
                dm.remove(sfPocket[0]);
                rank += dm.nRemaining() - board.size();
            } else {
                dm.remove(sfPocket);
                ++rank;
            }
        }
    }


    // set references
    const suitSet& ss(board.suitGroups());
        // board indices grouped by suit [s d h c]
        // structure is [[index index ...] [index index ...] etc.]

    const valueSet& sv(board.sortedValues());
    const valueSet& svo(board.sortedValuesCount());
    
    valueSet flushValues;
    suitSet::const_iterator ssIter(ss.begin());
    while (ssIter != ss.end()) {
        if (ssIter->size() > 2) {
            flushValues.resize(ssIter->size());
            const boardIndexSet flushIndices(*ssIter);
            boardIndexSet::const_iterator fiIter;
            for (
                fiIter = flushIndices.begin();
                fiIter != flushIndices.end();
                ++fiIter) {
                flushValues.push_back(
            }
            break;
        }
        ++ssIter;
    }
    
    if (flushPossible) {
        flushValues.setSiz
    }
    
    const valueSet& recurrences(board.valueGroupsByGroupSize());
        // cards with matching number, sorted by:
        //  * number of cards (i.e. four of a kind, set, pair, single)
        //  * value of cand number (i.e. with 2 pair showing, highest is first)
        // given as list of board indices
        // Structure is [value [index index ...] value [index index ...] ...]
    const valueSet& sortedValues(board.valueGroupsByValue());

    deskMask dm;
    dm.remove(board);
    int rank = 0;
    cardSet threeSuited(board.getThreeSuited());
    cardSuit suit(threeSuited[0].suit());
    if (threeSuited.size()){
        vector<tuple<cardValue> > completers
        (
            getStraightCompleters(threeSuited)
        );
        // iterate through straightFlushCompleters
	    vector<tuple<cardValue> >::const_iterator itc;
	    for (itc=completers.begin(); itc != completers.end(); itc++){
	        cardSet iCompleter(2
		    itc->printCard();
	    }
    }

    // get all straight completers now
    const valueSet findStraightCompleters(board);
        // indices of board cards that could form straights, along with the
        // required card values, sorted by:
        //  * highest value board card
        //  456, 457
}




getStraightFlushesFromThree(cardSet& threeSuited)
{
    card maxCard = max(threeSuited);
    card minCard = min(threeSuited);
    int spread(maxCard.value() - minCard.value());
    switch (spread)







    {
        case 2:
        {
            // All connected
            //    At edge
            //        123, JQK
            //            1 possible form
            //    One-off edge
            //        234, TJQ
            //            2 possible forms
            //    Midrange
            //        345, 456, ... 9TJ
            //            3 possible forms
            return forms;
        }
        case 3:
        {
            // One inside missing
            //    At edge
            //        124, 134, TJK TQK
            //            1 possible form
            //    Midrange
            //        235, 245, 346, 356, ... 89J, 8TJ, 9TQ, 9JQ
            //            2 possible forms
            return forms;
        }
        case 4:
        {
            // Two inside missing
            //    Edge cases don't matter
            //        125, 135, 145, 236, 246, 256 ... 9TK, 9JK, 9QK
            //            1 possible form
            return forms;
        }
        defalt:
        {
            return forms; // empty
        }
    }
}

void ds::handRanker::execute()
{
    std::cout << "Hello world" << std::endl;
}
