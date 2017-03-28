#ifndef pocketCards_h
#define pocketCards_h

#include<utility>
#include<card.H>
#include<pocketValues.h>

namespace ds {

class pocketCards;
std::ostream& operator<<(std::ostream& os, const pocketCards& c);
std::istream& operator>>(std::istream& in, pocketCards& c);

class pocketCards
:
    public std::pair<card>
{
public:


    // Constructors

        //- Construct from cards
        pocketCards(const card& cardA, const card& cardB):
            std::pair<card>(std::pair<card>(cardA, cardB))
//            hasWildValue_(cardA.hasWildValue() || cardB.hasWildValue()),
//            hasWildSuit_(cardA.hasWildSuit() || cardB.hasWildSuit())
        {}

        //- Construct from values and suits
        pocketCards
        (
            short valA,
            card::suitEnum suitA,
            short valB,
            card::suitEnum suitB
        ):
            std::pair(card(valA, suitA), card(valB, suitB))
        {
//            hasWildValue_(first.hasWildValue() || second.hasWildValue());
//            hasWildSuit_(first.hasWildSuit() || second.hasWildSuit());
        }


    //- Destructor
    ~pocketCards() {}


    // Member operators
    
        //- True if both pocket cards are the same as the given
        bool operator==(const pocketCards& pc) {
            return (
                first == pc.first && second == pc.second
            ) || (
                second == pc.first && first == pc.second
            );
        }

        //- False if either pocket cards differ from given
        bool operator!=(const pocketCards& pc) {
            return !(operator==(pc));
        }


private:

    // Private member data
    
        //- True if one of my cards is wild value
//        bool hasWildValue_;
        
        //- True if one of my cards is wild suited
//        bool hasWildSuit_;
        

    // Private member functions
    
//////////

};

}
#endif
