#ifndef PktCards_h
#define PktCards_h

#include<Card.h>
#include<PktVals.h>

namespace ds {

class PktCards;
typedef std::vector<PktCards> VecPktCards;

class PktCards
:
    public std::pair<Card, Card>
{
public:


    // Constructors

        //- Construct null
        PktCards();

        //- Construct from cards
        PktCards(const Card& cardA, const Card& cardB);

        //- Construct from values and suits
        PktCards
        (
            CardVal valA,
            Suit suitA,
            CardVal valB,
            Suit suitB
        );
        
        //- Construct from card, value and suit
        PktCards
        (
            const Card& CardA,
            CardVal valB,
            Suit suitB
        );

        //- Construct from value, suit and card
        PktCards
        (
            CardVal valA,
            Suit suitA,
            const Card& CardB
        );
        
        //- Construct from deck indices
        PktCards(DeckInd diA, DeckInd diB);

        //- Construct from VecDeckInd
        PktCards(VecDeckInd vdi);

        //- Construct from istream
        PktCards(std::istream& is);


    //- Destructor
    ~PktCards() {}

    
    // Member functions
    
        // Tests
        
            //- Returns true if pocket has given card
            bool has(const Card& cd) const;
            
            //- Returns true if pocket has given value
            bool has(CardVal cv) const;
            
            //- Returns true if pocket has given suit
            bool has(Suit st) const;

            //- Returns true if pocket is suited
            bool suited() const;

            //- Returns true if pocket is suited with given suit
            bool suited(Suit st) const;

            //- Returns true if pocket is a pair
            bool pairs() const;

            //- Returns true if pocket pair value is given value
            bool pairs(CardVal cv) const;


        // Derived data
        
            //- Returns highest value
            CardVal highestVal() const;


        // Edit
        
            //- Switch pocket cards first to second
            void swap();
            
            //- Ensure first.value() > second.value()
            void sort();


        // Conversions
            
            //- Return PktVals with suit stripped out
            PktVals values() const;


    // Member operators
    
        //- True if both pocket cards are the same as the given
        bool operator==(const PktCards& pc) const;

        //- False if either pocket cards differ from given
        bool operator!=(const PktCards& pc);
        
        //- Cast to VecCard
        operator VecCard();
};


// Global functions

//- True if both pocket cards are the same as the given, with no
//  checking for wild
bool noWildEquals(const PktCards& pcA, const PktCards& pcB);


} // End namespace ds
#endif
