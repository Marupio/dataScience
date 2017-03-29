#ifndef PocketCards_h
#define PocketCards_h

#include<Card.H>

namespace ds {

class PocketCards;
std::ostream& operator<<(std::ostream& os, const PocketCards& c);
std::istream& operator>>(std::istream& in, PocketCards& c);

class PocketCards
:
    public std::pair<Card>
{
public:


    // Constructors

        //- Construct from cards
        PocketCards(const Card& cardA, const Card& cardB):
            std::pair<Card>(std::pair<Card>(cardA, cardB))
        {}

        //- Construct from values and suits
        PocketCards
        (
            CardVal valA,
            Suit suitA,
            CardVal valB,
            Suit suitB
        ):
            std::pair(Card(valA, suitA), Card(valB, suitB))
        {}
        
        //- Construct from deck indices
        PocketCards(DeckInd diA, DeckInd diB):
            std::pair(Card(diA), Card(diB))
        {}

        PocketCards(std::istream& is):
            std::pair(Card(is), Card(is))
        {}


    //- Destructor
    ~PocketCards() {}


    // Member operators
    
        //- True if both pocket cards are the same as the given
        bool operator==(const PocketCards& pc) {
            return (
                first == pc.first && second == pc.second
            ) || (
                second == pc.first && first == pc.second
            );
        }

        //- False if either pocket cards differ from given
        bool operator!=(const PocketCards& pc) {
            return !(operator==(pc));
        }

    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const PocketCards& c);
    friend std::istream& operator>>(std::istream& in, PocketCards& c);
};

}
#endif
