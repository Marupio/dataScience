#ifndef card_h
#define card_h

//#include<stdIostream.h>
#include<string>
#include<error.h>

namespace ds {

class card;
std::ostream& operator<<(std::ostream& os, const card& c);
std::istream& operator>>(std::istream& in, card& c);

// Global public typedefs
typedef signed char suitType;
typedef signed char binValueType;

class card
{
public:


    // Static data
    
        //- Static suit data
        static const suitType unknownSuit;      // -1
        static const suitType clubs;            // 0
        static const suitType diamonds;         // 1
        static const suitType hearts;           // 2
        static const suitType spades;           // 3
        static const suitType wildSuit;         // 4

        //- Static binary value data
        static const binValueType binUnknownValue;  // -1
        static const binValueType binTwo;           // 0
        static const binValueType binThree;         // 1
        static const binValueType binFour;          // 2
        static const binValueType binFive;          // 3
        static const binValueType binSix;           // 4
        static const binValueType binSeven;         // 5
        static const binValueType binEight;         // 6
        static const binValueType binNine;          // 7
        static const binValueType binTen;           // 8
        static const binValueType binJack;          // 9
        static const binValueType binQueen;         // 10
        static const binValueType binKing;          // 11
        static const binValueType binAce;           // 12
        static const binValueType binWildValue;     // 13

        //- Static integer value data
        static const short unknownValue;    // 0
        static const short lowAce;          // 1 reserved for low-ace cases
        static const short two;             // 2
        static const short three;           // 3
        static const short four;            // 4
        static const short five;            // 5
        static const short six;             // 6
        static const short seven;           // 7
        static const short eight;           // 8
        static const short nine;            // 9
        static const short ten;             // 10
        static const short jack;            // 11
        static const short queen;           // 12
        static const short king;            // 13
        static const short ace;             // 14
        static const short wildValue;       // 15


    // Constructors

        //- Construct null    
        card();

        //- Construct from integer value and suit
        card(short value, suitType suit);

        //- Construct from bin values
        card(binValueType value, suitType suit);
        
        //- Construct from human-readable char array
        card(const char* chStr);

        //- Construct from human-readable string
        card(const std::string& str);

        //- Construct from deck index
        card(short di);
        
        //- Construct from istream
        card(std::istream& is);
    

    //- Destructor
    ~card() {}


    // Member functions

        // Static conversions

            // Conversions for card value

                //- Convert human-readable char to internal value
                static binValueType readCharToBinValue(char value);

                //- Convert human-readable char to integer value
                static short readCharToValue(char value);

                //- Convert internal value to human-readable char
                static char binValueToWriteChar(binValueType value);

                //- Convert integer value to human-readable char
                static char valueToWriteChar(short value);

                //- Convert internal value to integer value
                static short binValueToValue(binValueType value);

                //- Convert integer value to internal value
                static binValueType valueToBinValue(short value);


            // Conversions for card suit

                //- Convert human-readable char to internal suit
                static suitType readCharToSuit(char suit);

                //- Convert internal suit to human-readable char
                static char suitToWriteChar(suitType suit);


            //- Convert given card to deck indexing
            static short cardToDeckIndex(const card& c);

            //- Convert deck index to card
            static card cardToDeckIndex(short di);


        // Access

            //- Return value
            short value() const {
                return binValueToValue(binValue_);
            }

            //- Return internal bin value
            binValueType binValue() const {
                return binValue_;
            }
            
            //- Return suit
            suitType suit() const {
                return suit_;
            }
            
            bool hasWildValue() const {
                return binValue_ == binWildValue;
            }
            
            bool hasWildSuit() const {
                return suit_ == wildSuit;
            }

            bool partsUnknown() const {
                return binValue_ == binUnknownValue || suit_ == unknownSuit;
            }


    // Operators
    
        bool operator<(const card& c1) {
            return c1.binValue_ < binValue_;
        }

        bool operator<=(const card& c1) {
            if (c1.binValue_ == binWildValue || binValue_ == binWildValue) {
                return true;
            }
            return c1.binValue_ <= binValue_;
        }
        
        bool operator>(const card& c1) {
            return c1.binValue_ > binValue_;
        }

        bool operator>=(const card& c1) {
            if (c1.binValue_ == binWildValue || binValue_ == binWildValue) {
                return true;
            }
            return c1.binValue_ >= binValue_;
        }
        
        bool operator==(const card& c1) {
            return
                (
                    (c1.binValue_ == binValue_)
                 || (c1.binValue_ == binWildValue)
                 || (binValue_ == binWildValue)
                ) && (
                    (c1.suit_ == suit_)
                 || (c1.suit_ == wildSuit)
                 || (suit_ == wildSuit)
                );
        }

        bool operator!=(const card& c1) {
            return !(operator==(c1));
        }


    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const card& c);
    friend std::istream& operator>>(std::istream& in, card& c);


private:

    //- Internal value of card
    binValueType binValue_;

    //- Internal suit of card
    suitType suit_;
};

}
#endif
