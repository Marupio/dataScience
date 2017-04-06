#ifndef PktMask_h
#define PktMask_h

#include<unordered_map>
#include<PktCards.h>

namespace ds {

class PktMask:
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

        //- Hash table for quick lookup of PktMask indices
        static const DeckIndHashTable deckIndicesHashTable_;

        // Pre-indexed tables for wild value or wild suit matching
        
            //- VS **
            //  One card tables, e.g. 4C **
            //      i.e. remove all pocket combos with 4C
            //      52 tables with 51 entries each = 2652
            static const VecVecDeckInd oneCardTables_;
            
            //- *S **
            //  One suit tables, e.g. *C **
            //      i.e. one card needs a given suit
            //      4 tables, 13x51 entries each = 4x663 = 2652
            static const VecVecDeckInd oneSuitTables_;
            
            //- V* **
            //  One value tables, e.g. 4* **
            //      i.e. one card needs a given value
            //      13 tables, 4x51 entries each = 13x204 = 2652
            static const VecVecDeckInd oneValTables_;

            //- *S *S
            //  Two suit tables, e.g. *H *H
            //      i.e. remove all pocket suited hearts pocket combos
            //      only suited pockets make sense
            //      4 tables with 13x12 entries each = 4x156 = 624
            static const VecVecDeckInd twoSuitTables_;

            //- V* V*
            //  Two value tables, e.g. 3* 4*
            //      i.e. suit doesn't matter, values do
            //      dissimilar numbers:
            //          13x12 tables with 16 entries
            //      pocket pairs:
            //          13 tables with 6 entries
            //  Structured as a hashTable with the key being the value pair
            //  pair(valA, valB), valA >= valB
            static const TwoValHashTable twoValTables_;

            //- V* **
            //- *S **
            //- VS **
            //- V* V*
            //- *S *S
            //- Missing but required:
            //  VS V*
            //  

    // Constructors

        //- Construct null    
        PktMask():
            std::vector<signed char>(1326, 1),
            nRemaining_(1326)
        {}

        //- Construct from Board
        PktMask(const Board& b):
            std::vector<signed char>(1326, 1),
            nRemaining(1326)
        {
            remove(b);
        }

        //- Construct from Board and player's PocketCards
        //  Note: players pocket cards are removed individually
        PktMask(const Board& b, const PktCards pc):
            std::vector<signed char>(1326, 1),
            nRemaining(1326)
        {
            remove(b);
            remove(pc.first());
            remove(pc.second());
        }


    //- Destructor
    ~PktMask() {}


    // Member functions

        // Access
        
            //- Return nRemaining
            short nRemaining() const {
                return nRemaining_
            }

        // Removal functions
        
            //- Remove any combinations with the given board cards
            short remove(const Board& b);

            //- Remove given pocket cards from possibilities
            //  Pocket cards must be sorted
            short remove(const PktCards& pc);

            //- Remove any combinations with the given card
            short remove(const Card& c);


private:

    // Private data
    
        //- Number of possibilities remaining
        short nRemaining_;


    // Private member functions
    
        //- Helper for remove(PktCards)
        short ds::PktMask::removeFromVecDeckInd(
            const VecDeckInd& table
        );

///////

        //- Construct from integer value and suit
        pocketMask(short value, suitType suit)

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
