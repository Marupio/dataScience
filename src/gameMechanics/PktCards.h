#ifndef PktCards_h
#define PktCards_h

#include <Card.h>
#include <PktVals.h>

namespace ds {

class PktCards;
typedef std::vector<PktCards> VecPktCards;

class PktCards
:
    public std::pair<Card, Card>
{
public:

    // Public static data

        //- Valid startingHandIndices
        //  i.e. compactToStartingHandIndex[compactStartingHandIndex] = startingHandIndex
        //  where:
        //  * startingHandIndex is from 0..300 with gaps
        //  * compactStartingHandIndex is from 0..168 with no gaps
        static const size_t nStartingHands = 169;
        static const std::array<size_t, 169> cshiToShi;
        static const std::array<size_t, 301> shiToCshi;


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

        //- Construct from VecToken
        PktCards(VecToken& vt);


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
        
            //- Returns highest value, not including avoid cards
            CardVal highestValue() const;
            CardVal highestValue(CardVal avoid) const;

            //- Returns starting hand name.  Format is:
            //  * Pairs: "88", "TT", etc.
            //  * Suited: "AKs", "T2s", etc.
            //  * Unsuited: "T9u", "72u", etc.
            std::string startingHandName() const;

            //- Returns a unique index value for a starting hand type
            //  For use with tables associated with starting hand type
            size_t startingHandIndex() const;

            //- Returns compact version of startingHandIndex
            size_t compactStartingHandIndex() const;

            //- Make starting hand from compact starting hand index (0..168)
            //  Uses clubs as suited, and clubs hearts as unsuited.
            static PktCards makeStartingHand(short compactStartingHandIndex);


        // Edit
        
            //- Switch pocket cards first to second
            void swap();
            
            //- Ensure first.value() > second.value()
            void sort();


        // Conversions
            
            //- Return PktVals with suit stripped out
            PktVals values() const;

            //- Return PktVals with suit stripped out, with largest value in first
            PktVals sortedValues() const;


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
