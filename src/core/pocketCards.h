#ifndef pocketCards_h
#define pocketCards_h

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
        {}

        //- Construct from values and suits
        pocketCards
        (
            short valA,
            card::suitType suitA,
            short valB,
            card::suitType suitB
        ):
            std::pair(card(valA, suitA), card(valB, suitB))
        {}
        
        //- Construct from deck indices
        pocketCards(short diA, short diB):
            std::pair(card(diA), card(diB))
        {}

        pocketCards(std::istream& is):
            std::pair(card(is), card(is))
        {}


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

    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const pocketCards& c);
    friend std::istream& operator>>(std::istream& in, pocketCards& c);
};

}
#endif
