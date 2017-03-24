#include<handRanker.h>
#include<iostream>

int getRank(const cardSet& board, const cardSet& pocket){
    // set references
    const suitSet& ss(board.suitGroups());
        // board indices grouped by suit [s d h c]
        // structure is [[index index ...] [index index ...] etc.]
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


typedef valueSet std::vector<short>;
typedef pocketValueSet std::tuple<short>;
typedef straightCompleters std::pair<valueSet, pocketValueSet>;

straightCompleters findStraightCompleters(const valueSet& values){
    straightCompleters sc;
    if (!values.size()) {
        return sc;
    }
    sc.reserve(5);
    
    short cursor = std::max(values.back() + 2, 14);
    pocketValueSet pocket(0, 0);
    

    vector<straightCompleter> completers;
    const valueSet& sortedValues(board.valueGroupsByValue());
    if (bccct) {
        if (topIsAce) {
            // TJQKA
            return ace (wild, wild);
        } else if (topIsKing) {
            // 9TJQK
            return
                ace (ace, wild)
                king (wild, wild);
        } else {
            // all others
            return
                top+2 (top+1, top+2)
                top+1 (top+1, wild)
                top (wild, wild);
        }
    } else if (bcct) {
        if (topIsAce) {
            // *JQKA
            return ace (bottom-1, wild);
        } else if (bottomIsAce) {
            // A234*
            return
                5 (5, wild);
        } else {
            // *bcct*
            return
                top+1 (top+1, wild)
                top (bottom-1, wild)
        }
    }
    else if (bxcct) {
        if (topIsAce) {
            // xT*QKA
            return
                ace (jack, wild);
        } else if (topIsKing) {
            // x9*JQK*
            return
                ace (10, ace)
                king (10, wild);
        } else {
            // bottomIsAce is not an edge case
            // xb*cct**
            return
                top+2 (top+1, top+2)
                top+1 (bottom+1, top+1);
                top (bottom+1, wild);
        }
    } else if (bcxct) {
        if (topIsAce) {
            // xTJ*KA
            return ace (queen, wild);
        } else if (topIsKing) {
            // x9T*QK*
            return
                ace (jack, ace)
                king (jack, wild);
        } else {
            // bottomIsAce is not an edge case
            // xbc*ct*
            return
                top+1 (bottom+2, top+1)
                top (bottom+2, wild);
        }
    } else if (bccxt) {
        if (topIsAce) {
            // **TJQ*A
            return
                ace (9, king)
                queen (8, 9);
        } else if (bottomIsAce) {
            // topIsKing is not an edge case
            // A23*5*
                return
                    6 (4, 6)
                    5 (4, wild);
        } else if (bottomIsTwo) {
            // x234*6*
            return
                7 (5, 7)
                6 (5, wild);
        } else {
            // **bcc*t*
            return
                top+1 (bottom+3, top+1)
                top (bottom+3, wild)
                bottom+2 (bottom-2, bottom-1);
        }
    }
    else if (bct) {
        if (topIsAce) {
            // **QKA
            return
                ace (10, jack);
        } else if (topIsKing) {
            // **JQK*
            return
                ace (10, ace)
                king (9, 10);
        } else if (bottomIsAce) {
            // A23**
            return
                5 (4, 5);
        } else if (bottomIsTwo) {
            // *234**
            return
                6 (5, 6)
                5 (ace, 5);
        } else {
            // **bct**
            return
                top+2 (top+1, top+2)
                top+1 (bottom-1, top+1)
                top (bottom-2, bottom-1);
        }
    }
    else if (bxct) {
        if (topIsAce) {
            // *J*KA
            return
                ace (10, queen);
        } else if (aceIsBottom) {
            // topIsKing is not an edge case
            // A*34*
            return
                5 (2, 5);
        } else {
            // twoIsBottom is not an edge case
            // *b*ct*
            return
                top+1 (bottom+1, top+1)
                top (bottom-1, bottom+1);
        }
    }
    else if (bcxt) {
        if (topIsAce) {
            // *JQ*A
            return
                ace (10, king);
        } else if (bottomIsAce) {
            // topIsKing is not an edge case
            // A2*4*
            return
                5 (3, 5);
        } else {
            // bottomIsTwo is not an edge case
            // *bc*t*
            return
                top+1 (bottom+2, top+1);
                top (bottom-1, bottom+2);
        }
    }
    else if (bxxct) {
        // no edge cases
        return
            top (bottom+1, bottom+2);
    }
    else if (bxcxt)
    {
        // no edge cases
        return
            top (bottom+1, bottom+3);
    }
    else if (bcxxt)
    {
        // no edge cases
        return
            top (bottom+2, bottom+3);
    }
// what about bxxccxxt
// check i5-i1 < 5 --> 5 consecutive
// else check i4-i1 < 5 --> 4straight, check extra card:
    // bcct: bcct*e bcct**e
    // b*cct: b*ccte b*cct*e b*cct**e
    // bc*ct: bc*cte bc*ct*e bc*ct**e
    // bcc*t: e**bcc*te e**bcc*t*e
// else check i5-i2 < 5 --> 4straight, check extra card:
    // bcct: (no extra cases)
    // b*cct: (no extra cases)
    // bc*ct: e**bc*ct e*bc*ct ebc*ct
    // bcc*t: ebcc*t e*bcc*t e**bcc*t
// else check i3-i1 < 5 --> 3straight, check extra card:
    // bct: bct*e bct**e
    // b*ct: b*cte
    // bc*t
    // b**ct
    // b*c*t
    // bc**t
// 
// check i3-i1 < 4
// check i


// New algorithm:
// Start from max on board + 2, and count backwards
// Throw away candidates when more than 2 additional numbers are required
// When we reach 5 using two cards, add them to the list with the max value
// When we reach 5:
//  * using 2 cards, add them to the list + max card
//  * using 1 card, add it and wild to the list + max card
//  * using no cards, add wild wild to the list + max card
// Throw away top card and resume
// Account for straights that run into one another, thus eliminating lower end



    // check for 5 straight
        // check for top is ace
            // return wild cards
        // check for top is king
            // return ace *
        //
    if (sortedValues.size() == 5){
        if (
            sortedValues[4] - sortedValues[0] == 4 // all but A2345
         || (sortedValues[4] == 14 && sortedValues[3] == 5) // A2345
        ){
            if (sortedValues[3] == 13)
            {
                // No higher straight, return two wild values
                vector<char> boardIndices(5);
                std::iota(boardIndices.begin(), boardIndices.end(), 0);
                vector<tuple<short>> pocketValues(1, tuple<short>(1, 1));
                vector<short> maxValues(1, 14);
                straightCompleter sc(boardIndices, pocketValues, maxValues);
                completers.push_back(sc);
                return completers;
            }
            // One from top
            if (sortedValues[3] == 12)
            {
                // Only an ace gives a higher straight
                {
                    vector<char> boardIndices(4);
                    std::iota(boardIndices.begin(), boardIndices.end(), 1);
                    vector<tuple<short>> pocketValues(1, tuple<short>(14, 1));
                    vector<short> maxValues(1, 14);
                    straightCompleter sc(boardIndices, pocketValues, maxValues);
                    completers.push_back(sc);
                }
                {
                    vector<char> boardIndices(5);
                    std::iota(boardIndices.begin(), boardIndices.end(), 0);
                    vector<tuple<short>> pocketValues(1, tuple<short>(1, 1));
                    vector<short> maxValues(1, 13);
                    straightCompleter sc(boardIndices, pocketValues, maxValues);
                    completers.push_back(sc);
                }
                return straightCompleters;
            }
I don't like this structure
            {
                vector<char> boardIndices(4);
                std::iota(boardIndices.begin(), boardIndices.end(), 1);
                vector<tuple<short>> pocketValues(1, tuple<short>(14, 1));
                vector<short> maxValues(1, 14);
                straightCompleter sc(boardIndices, pocketValues, maxValues);
                completers.push_back(sc);
            }
            {
                vector<char> boardIndices(5);
                std::iota(boardIndices.begin(), boardIndices.end(), 0);
                vector<tuple<short>> pocketValues(1, tuple<short>(1, 1));
                vector<short> maxValues(1, 13);
                straightCompleter sc(boardIndices, pocketValues, maxValues);
                completers.push_back(sc);
            }
            // Two or more from top
            
            
        }
        // Check for 4 consecutive values
        
    }

    {
        const char maxValue (sortedValues.end()
    }
    // check for full straight
        // if full straight
            // check top edge cases
                // one from edge
            // else two from edge
            // then own from edge
            // then all else

    // check board for full straight:
        // only return those on the up end
    // check bord for four in a straght
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
