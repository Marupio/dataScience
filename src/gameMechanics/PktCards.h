#ifndef PktCards_h
#define PktCards_h

#include<Card.h>

namespace ds {

class PktCards;
//std::ostream& operator<<(std::ostream& os, const PktCards& c);
//std::istream& operator>>(std::istream& in, PktCards& c);

typedef std::vector<PktCards> VecPktCards;

class PktCards
:
    public std::pair<Card, Card>
{
public:


    // Constructors

        //- Construct null
        PktCards():
            std::pair<Card, Card>(Card::unknownCard, Card::unknownCard)
        {}

        //- Construct from cards
        PktCards(const Card& cardA, const Card& cardB):
            std::pair<Card, Card>(cardA, cardB)
        {}

        //- Construct from values and suits
        PktCards
        (
            CardVal valA,
            Suit suitA,
            CardVal valB,
            Suit suitB
        ):
            std::pair<Card, Card>(Card(valA, suitA), Card(valB, suitB))
        {}

        //- Construct from card, value and suit
        PktCards
        (
            const Card& CardA,
            CardVal valB,
            Suit suitB
        ):
            std::pair<Card, Card>(CardA, Card(valB, suitB))
        {}

        //- Construct from value, suit and card
        PktCards
        (
            CardVal valA,
            Suit suitA,
            const Card& CardB
        ):
            std::pair<Card, Card>(Card(valA, suitA), CardB)
        {}
        
        //- Construct from deck indices
        PktCards(DeckInd diA, DeckInd diB):
            std::pair<Card, Card>(Card(diA), Card(diB))
        {}

        //- Construct from VecDeckInd
        PktCards(VecDeckInd vdi):
            std::pair<Card, Card>(Card::unknownCard, Card::unknownCard)
        {
            if (vdi.size() != 2) {
                FatalError << "Attempting to construct PktCards from "
                    << "VecDeckInd with size " << vdi.size() << std::endl;
                abort();
            }
            first = Card(vdi.front());
            second = Card(vdi.back());
        }

        //- Construct from istream
        PktCards(std::istream& is) {
            is >> *this;
        }


    //- Destructor
    ~PktCards() {}

    
    // Member functions
    
        //- Returns true if pocket has given card
        bool has(const Card& cd) const {
            return first == cd || second == cd;
        }
        
        //- Returns true if pocket has given value
        bool has(CardVal cv) const {
            return first.value() == cv || second.value() == cv;
        }
        
        //- Returns true if pocket has given suit
        bool has(Suit st) const {
            return first.suit() == st || second.suit() == st;
        }

        //- Returns true if pocket is suited
        bool suited() const {
            return first.suit() == second.suit();
        }

        //- Returns true if pocket is suited with given suit
        bool suited(Suit st) const {
            return first.suit() == st && second.suit() == st;
        }

        //- Returns true if pocket is a pair
        bool pairs() const {
            return first.value() == second.value();
        }

        //- Returns true if pocket pair value is given value
        bool pairs(CardVal cv) const {
            return first.value() == cv && second.value() == cv;
        }


    // Member operators
    
        //- True if both pocket cards are the same as the given
        bool operator==(const PktCards& pc) const {
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
        
        //- Cast to VecCard
        operator VecCard() {
            return {first, second};
        }


    // Friend functions
//    friend std::ostream& operator<<(std::ostream& out, const PktCards& c);
//    friend std::istream& operator>>(std::istream& in, PktCards& c);
};

} // End namespace ds
#endif
