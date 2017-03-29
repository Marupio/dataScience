#ifndef pocketMask_h
#define pocketMask_h

#include<pocketCards.h>
#include<error.h>

/*
    Has all the combinations of pocket cards by desk index
        52x51=2652 entries
    Has static tables for desk indices for given wilds
        Case Val Suit | Val Suit    * = wild
         A    *
         B         *
         C    *    *
         D         *          *
         E    *          *
         F    *    *     *
         G    *    *          *
         H    *    *     *    *

        ** A **
            e.g. 2C *C
            I don't think we will need this one

        ** B **
            e.g. 3S 4*
            I don't think we will need this one

        ** C ** oneCardTables
            e.g. 4H ** ... i.e. remove all pocket combos with 4H
            52 tables with 51 entries each = 2652
            
        ** D ** twoSuitTables
            e.g. *C *C ... i.e. remove all suited clubs combos
            only suited tables make any sense.
            4 tables with 13x12 entries each = 4x156 = 624
        
        ** E ** twoValTables, pocketPairTables
            e.g. 3* 4* ... i.e. suit doesn't matter, values do
            dissimilar values:
                13x12 tables with 16 entries each
                    but we can reduce this by eliminating > 5 difference
                    13x8 tables with 16 entries each, must account for low ace
                        = 104x16 = 1664
            identical values:
                13 tables with 6 entries each = 78
            
        ** F ** oneSuitTables
            e.g. ** *C ... i.e. one card needs a given suit
            4 tables, 13x51 entries each = 4x663 = 2652
            
        ** G ** oneValTables
            e.g. ** 4* ... i.e. one card needs a given value
            13 tables, 4x51 entries each = 13x204 = 2652
        Total 11647 shorts at 2 bytes each, 22 kb of static data
*/

#include<unordered_map>

namespace ds {

class pocketMask:
    public std::vector<signed char>
{
public:

    // Public typedefs

    typedef std::unordered_map<PktDeckInd, DeckInd> DeckIndHashTable;
    typedef std::unordered_map<PktDeckInd, VecDeckInd> TwoValHashTable;

    // Public static data

        //- Deck index pairs associated with this class' vector
        //      52x51 = 1325
        static const VecPktDeckInd deckIndices_;

        //- Hash table for quick lookup of 

        // Pre-indexed tables for wild value or wild suit matching
        
            //- One card tables, e.g. 4C **
            //      i.e. remove all pocket combos with 4C
            //      52 tables with 51 entries each = 2652
            static const VecVecDeckInd oneCardTables_;
            
            //- One suit tables, e.g. ** *C
            //      i.e. one card needs a given suit
            //      4 tables, 13x51 entries each = 4x663 = 2652
            static const VecVecDeckInd oneSuitTables_;
            
            //- One value tables, e.g. ** 4*
            //      i.e. one card needs a given value
            //      13 tables, 4x51 entries each = 13x204 = 2652
            static const VecVecDeckInd oneValTables_;

            //- Two suit tables, e.g. *H *H
            //      i.e. remove all pocket suited hearts pocket combos
            //      only suited pockets make sense
            //      4 tables with 13x12 entries each = 4x156 = 624
            static const VecVecDeckInd twoSuitTables_;

            //- Two value tables, e.g. 3* 4*
            //      i.e. suit doesn't matter, values do
            //      dissimilar numbers:
            //          13x12 tables with 16 entries
            //      pocket pairs:
            //          13 tables with 6 entries
            //  Structured as a hashTable with the key being the value pair
            //  pair(valA, valB), valA >= valB
            static const TwoValHashTable twoValTables_;

/////////////////
    // Constructors

        //- Construct null    
        pocketMask();

        //- Construct from integer value and suit
        pocketMask(short value, suitType suit);

        //- Construct from bin values
        pocketMask(binValueType value, suitType suit);
        
        //- Construct from human-readable char array
        pocketMask(const char* chStr);

        //- Construct from human-readable string
        pocketMask(const std::string& str);

        //- Construct from deck index
        pocketMask(short di);
        
        //- Construct from istream
        pocketMask(std::istream& is);
    

    //- Destructor
    ~pocketMask() {}


    // Member functions

        // Static conversions

            // Conversions for pocketMask value

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


            // Conversions for pocketMask suit

                //- Convert human-readable char to internal suit
                static suitType readCharToSuit(char suit);

                //- Convert internal suit to human-readable char
                static char suitToWriteChar(suitType suit);


            //- Convert given pocketMask to deck indexing
            static short cardToDeckIndex(const pocketMask& c);

            //- Convert deck index to pocketMask
            static pocketMask cardToDeckIndex(short di);


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
    
        bool operator<(const pocketMask& c1) {
            return c1.binValue_ < binValue_;
        }

        bool operator<=(const pocketMask& c1) {
            if (c1.binValue_ == binWildValue || binValue_ == binWildValue) {
                return true;
            }
            return c1.binValue_ <= binValue_;
        }
        
        bool operator>(const pocketMask& c1) {
            return c1.binValue_ > binValue_;
        }

        bool operator>=(const pocketMask& c1) {
            if (c1.binValue_ == binWildValue || binValue_ == binWildValue) {
                return true;
            }
            return c1.binValue_ >= binValue_;
        }
        
        bool operator==(const pocketMask& c1) {
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

        bool operator!=(const pocketMask& c1) {
            return !(operator==(c1));
        }


    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const pocketMask& c);
    friend std::istream& operator>>(std::istream& in, pocketMask& c);


private:

    //- Internal value of pocketMask
    binValueType binValue_;

    //- Internal suit of pocketMask
    suitType suit_;
};

}
#endif
