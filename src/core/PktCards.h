#ifndef PktCards_h
#define PktCards_h

#include<Card.H>

namespace ds {

class PktCards;
std::ostream& operator<<(std::ostream& os, const PktCards& c);
std::istream& operator>>(std::istream& in, PktCards& c);

class PktCards
:
    public std::pair<Card>
{
public:


    // Constructors

        //- Construct from cards
        PktCards(const Card& cardA, const Card& cardB):
            std::pair<Card>(std::pair<Card>(cardA, cardB))
        {}

        //- Construct from values and suits
        PktCards
        (
            CardVal valA,
            Suit suitA,
            CardVal valB,
            Suit suitB
        ):
            std::pair(Card(valA, suitA), Card(valB, suitB))
        {}
        
        //- Construct from deck indices
        PktCards(DeckInd diA, DeckInd diB):
            std::pair(Card(diA), Card(diB))
        {}

        PktCards(std::istream& is):
            std::pair(Card(is), Card(is))
        {}


    //- Destructor
    ~PktCards() {}


    // Member operators
    
        //- True if both pocket cards are the same as the given
        bool operator==(const PktCards& pc) {
            return (
                first == pc.first && second == pc.second
            ) || (
                second == pc.first && first == pc.second
            );
        }

        //- False if either pocket cards differ from given
        bool operator!=(const PktCards& pc) {
            return !(operator==(pc));
        }

    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const PktCards& c);
    friend std::istream& operator>>(std::istream& in, PktCards& c);
};

}
#endif
