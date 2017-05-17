// TODO - Get rid of binCardValue and just use value alone.

#ifndef Card_h
#define Card_h

#include <string>
#include <types.h>
#include <Suit.h>

namespace ds {

class Card;
std::ostream& operator<<(std::ostream& os, const Card& c);
std::istream& operator>>(std::istream& in, Card& c);

typedef std::vector<Card> VecCard;
typedef std::vector<Suit> VecSuit;

class Card
{
public:


    // Static data

        //- Static card data
        static const Card unknownCard;
        static const Card wildCard;
    
        //- Static suit data
        static const short nSuits;
        
        static const Suit unknownSuit;      // -2
        static const Suit wildSuit;         // -1
        static const Suit clubs;            // 0
        static const Suit diamonds;         // 1
        static const Suit hearts;           // 2
        static const Suit spades;           // 3

        //- Static integer value data
        static const CardVal unknownValue;    // -1
        static const CardVal wildValue;       // 0
        static const CardVal lowAce;          // 1 reserved for low-ace cases
        static const CardVal two;             // 2
        static const CardVal three;           // 3
        static const CardVal four;            // 4
        static const CardVal five;            // 5
        static const CardVal six;             // 6
        static const CardVal seven;           // 7
        static const CardVal eight;           // 8
        static const CardVal nine;            // 9
        static const CardVal ten;             // 10
        static const CardVal jack;            // 11
        static const CardVal queen;           // 12
        static const CardVal king;            // 13
        static const CardVal ace;             // 14

        //- Words for each card value
        static const std::array<std::string, 15> CardValNames;
        static const std::array<std::string, 15> CardValNamesPlural;

        //- Words for each card suit
        static const std::array<std::string, 4> CardSuitNames;


    // Constructors

        //- Construct null
        Card();

        //- Construct from integer value and suit
        Card(CardVal value, Suit suit);

        //- Construct from human-readable char array
        Card(const char* chStr);

        //- Construct from human-readable string
        Card(const std::string& str);

        //- Construct from deck index
        Card(DeckInd di);
        
        //- Construct from istream
        Card(std::istream& is);
    

    //- Destructor
    ~Card() {}


    // Member functions

        // Static conversions

            // Conversions for card value

                //- Convert human-readable char to card value
                static CardVal readCharToValue(char value);

                //- Convert card value to human-readable char
                static char valueToWriteChar(CardVal value);


            // Conversions for card suit

                //- Convert human-readable char to internal suit
                static Suit readCharToSuit(char suit);

                //- Convert internal suit to human-readable char
                static char suitToWriteChar(Suit suit);


            //- Convert given card to deck indexing
            static DeckInd cardToDeckIndex(const Card& c);

            //- Convert deck index to card
            static Card deckIndexToCard(DeckInd di);


        // Access

            //- Return card value
            CardVal value() const;

            //- Return suit
            Suit suit() const;
            
            //- True if value is wild
            bool hasWildValue() const;
            
            //- True if suit is wild
            bool hasWildSuit() const;

            //- True if value or suit is wild
            bool hasWild() const;

            //- True if value and suit are wild
            bool wild() const;

            //- Checks validity of card and returns true if valid
            bool valid() const;
            
            //- True if card represents a physical card with no wild components
            //  and does not have a lowAce value
            bool real() const;

            //- True if value or suit is unknown
            bool partsUnknown() const;

            //- Return my deck index
            DeckInd deckIndex() const;


    // Operators
    
        bool operator<(const Card& c1) const;

        bool operator<=(const Card& c1) const;
        
        bool operator>(const Card& c1) const;
        
        bool operator>=(const Card& c1) const;
        
        bool operator==(const Card& c1) const;

        bool operator!=(const Card& c1) const;


    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const Card& c);
    friend std::istream& operator>>(std::istream& in, Card& c);


private:

    //- Internal value of card
    CardVal value_;

    //- Internal suit of card
    Suit suit_;
};


// Global functions

//- Returns true if CardA == CardB without accounting for wild
bool noWildEquals(const Card& cA, const Card& cB);

} // end namespace ds

#endif
