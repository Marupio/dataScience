#ifndef Card_h
#define Card_h

#include<string>
#include<error.h>
#include<types.h>

namespace ds {

class Card;
std::ostream& operator<<(std::ostream& os, const Card& c);
std::istream& operator>>(std::istream& in, Card& c);

typedef std::pair<Card, Card> PktCards;
typedef std::vector<PktCards> VecPktCards;
typedef std::vector<Card> VecCards;

class Card
{
public:


    // Static data
    
        //- Static suit data
        static const Suit unknownSuit;      // -1
        static const Suit clubs;            // 0
        static const Suit diamonds;         // 1
        static const Suit hearts;           // 2
        static const Suit spades;           // 3
        static const Suit wildSuit;         // 4

        //- Static binary value data
        static const BinCardVal binUnknownValue;  // -1
        static const BinCardVal binTwo;           // 0
        static const BinCardVal binThree;         // 1
        static const BinCardVal binFour;          // 2
        static const BinCardVal binFive;          // 3
        static const BinCardVal binSix;           // 4
        static const BinCardVal binSeven;         // 5
        static const BinCardVal binEight;         // 6
        static const BinCardVal binNine;          // 7
        static const BinCardVal binTen;           // 8
        static const BinCardVal binJack;          // 9
        static const BinCardVal binQueen;         // 10
        static const BinCardVal binKing;          // 11
        static const BinCardVal binAce;           // 12
        static const BinCardVal binWildValue;     // 13

        //- Static integer value data
        static const CardVal unknownValue;    // 0
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
        static const CardVal wildValue;       // 15


    // Constructors

        //- Construct null    
        Card();

        //- Construct from integer value and suit
        Card(CardVal value, Suit suit);

        //- Construct from bin values
        Card(BinCardVal value, Suit suit);
        
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

                //- Convert human-readable char to internal value
                static BinCardVal readCharToBinValue(char value);

                //- Convert human-readable char to card value
                static CardVal readCharToValue(char value);

                //- Convert internal value to human-readable char
                static char binValueToWriteChar(BinCardVal value);

                //- Convert card value to human-readable char
                static char valueToWriteChar(CardVal value);

                //- Convert internal value to card value
                static CardVal binValueToValue(BinCardVal value);

                //- Convert integer value to internal value
                static BinCardVal valueToBinValue(CardVal value);


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

            //- Return internal bin value
            BinCardVal binValue() const;
            
            //- Return suit
            Suit suit() const;
            
            //- True if value is wild
            bool hasWildValue() const;
            
            //- True if suit is wild
            bool hasWildSuit() const;

            //- True if value or suit is unknown
            bool partsUnknown() const;

            //- Return my deck index
            DeckInd deckIndex() const;


    // Operators
    
        bool operator<(const Card& c1);

        bool operator<=(const Card& c1);
        
        bool operator>(const Card& c1);
        
        bool operator>=(const Card& c1);
        
        bool operator==(const Card& c1);

        bool operator!=(const Card& c1);


    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const Card& c);
    friend std::istream& operator>>(std::istream& in, Card& c);


private:

    //- Internal value of card
    BinCardVal binValue_;

    //- Internal suit of card
    Suit suit_;
};

} // end namespace ds

#include<CardI.h>

#endif
