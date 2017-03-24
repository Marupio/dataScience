#ifndef card_h
#define card_h

//#include<stdIostream.h>
#include<error.h>

namespace ds {

class card;
std::ostream& operator<<(std::ostream& os, const card& c);
std::istream& operator>>(std::istream& in, card& c);

class card
{
public:

    // public data types
    enum suitEnum
    {
        UNKNOWN,
        WILD,
        CLUBS,
        DIAMONDS,
        HEARTS,
        SPADES
    };


    // Constructors

        //- Construct null    
        card():
            value_(0),
            suit_(UNKNOWN)
        {}

        //- Construct from components
        card(short value, suitEnum suit):
            value_(value),
            suit_(suit)
        {}

        //- Construct from chars
        card(char value, char suit):
            value_(charToValue(value)),
            suit_(charToSuit(suit))
        {}
        
        //- Construct from istream
        card(std::istream& is):
            value_(0),
            suit_(UNKNOWN)
        {
            is >> *this;
        }
    

    //- Destructor
    ~card() {}


    // Member functions

        // Static conversions

            //- Convert char to value
            static short charToValue(char value);
            
            //- Convert value to char
            static char valueToChar(short value);
            // Not yet implemented

            //- Convert char to suit
            static suitEnum charToSuit(char suit);

            //- Convert suitToChar
            static char suitToChar(suitEnum suit);
            // not yet implemented

        // Access

            //- Return value
            short value() const {
                return value_;
            }
            
            //- Return suit
            suitEnum suit() const {
                return suit_;
            }


    // Operators
    
        bool operator<(const card& c1) {
            return c1.value_ < value_;
        }

        bool operator<=(const card& c1) {
            return c1.value_ <= value_;
        }
        
        bool operator>(const card& c1) {
            return c1.value_ > value_;
        }

        bool operator>=(const card& c1) {
            return c1.value_ >= value_;
        }
        
        bool operator==(const card& c1) {
            return c1.value_ == value_;
        }


    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const card& c);
    friend std::istream& operator>>(std::istream& in, card& c);


private:

    //- Value of card:
    //  0 = unknown
    //  1 = wild
    //  2 = 2
    //  3 = 3
    //  ...
    //  13 = king
    //  14 = ace
    short value_;

    //- Suit of card
    suitEnum suit_;
};

}
#endif
