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
