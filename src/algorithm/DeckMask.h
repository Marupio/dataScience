// DeckMask
// A vector of all the DeckIndices that are possibly in the deck.
// This class uses insert and erase functions of std::vector, making it look
// ineffecient. However, DeckMask is used by repeatedly iterating over its
// contents. The inefficiencies in this class are more than made up for in
// its use.

#ifndef DeckMask_h
#define DeckMask_h

#include <types.h>
#include <Board.h>
#include <PktCards.h>

namespace ds {

class DeckMask:
    public VecDeckInd
{

public:

    // Constructors    

        //- Construct null
        DeckMask();
        
        //- Construct given board
        DeckMask(const Board& bd);
        
        //- Construct given pocket
        DeckMask(const PktCards& pkt);

        //- Construct given board and pocket
        DeckMask(const Board& bd, const PktCards& pkt);

        //- Construct given VecDeckInd to be removed
        DeckMask(const VecDeckInd& vdi);


    // Public Member Functions

        //- Remove board
        short remove(const Board& bd);

        //- Remove pkt
        short remove(const PktCards& pkt);
        
        //- Remove card
        short remove(const Card& c);
        
        //- Remove VecDeckInd
        short remove(const VecDeckInd& vdi);
        
        //- Remove deck index
        short remove(DeckInd di);


    // Private Member Functions

        //- Build a full set of deck indices
        void initialiseIndices();
    
};


} // End namespace ds

#endif

