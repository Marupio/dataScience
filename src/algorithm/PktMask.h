#ifndef PktMask_h
#define PktMask_h

#include<unordered_map>
#include<PktCards.h>
#include<Board.h>

namespace ds {

class PktMask:
    public std::vector<short>
{
public:

    // Public typedefs

//    typedef std::unordered_map<PktDeckInd, DeckInd> DeckIndHashTable;

    // Public static data

        //- Deck index pairs associated with this class' vector
        //      52x51 = 1325
        static const VecPktDeckInd deckIndices_;

        //- Given two known cards with deck indices diA > diB,
        //      deckIndicesToPocketMask[hash] = pocketMaskIndex of this pair
        //      where:
        //          hash = diA + diB*51
        static const VecDeckInd deckIndicesToPocketMask_;

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
            //      twoValTables_[i] gives all entries with two vals, where:
            //          i = (valA-2) + (valB-2)*12
            //          and valA > valB
            static const VecVecDeckInd twoValTables_;

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
            std::vector<short>(1326, 1),
            nRemaining_(1326)
        {}

        //- Construct from Board
        PktMask(const Board& b):
            std::vector<short>(1326, 1),
            nRemaining_(1326)
        {
            remove(b);
        }

        //- Construct from Board and player's PocketCards
        //  Note: players pocket cards are removed individually
        PktMask(const Board& b, const PktCards pc):
            std::vector<short>(1326, 1),
            nRemaining_(1326)
        {
            remove(b);
            remove(pc.first);
            remove(pc.second);
        }


    //- Destructor
    ~PktMask() {}


    // Member functions

        // Access
        
            //- Return nRemaining
            short nRemaining() const {
                return nRemaining_;
            }

        // Removal functions
        
            //- Remove any combinations with the given board cards
            short remove(const Board& b);

            //- Remove given pocket cards from possibilities
            //  Pocket cards must be sorted
            short remove(PktCards pc);

            //- Remove any combinations with the given card
            short remove(const Card& c);


private:

    // Private data
    
        //- Number of possibilities remaining
        short nRemaining_;


    // Private member functions
    
        //- Remove from index
        short removeIndex(short maskIndex) {
            #ifdef DSDEBUG
            short removed = at(maskIndex);
            operator[](maskIndex) = 0;
            return removed;
            #else
            short removed = operator[](maskIndex);
            operator[](maskIndex) = 0;
            return removed;
            #endif
        }
    
        //- Helper for remove(PktCards)
        short removeFromVecDeckInd(const VecDeckInd& table);
};

}
#endif
