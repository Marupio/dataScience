#include<cctype>
#include<PktMask.h>

// Static data

#include<PktMaskDeckIndices.h>
#include<PktMaskDeckIndicesHashTable.h>
#include<PktMaskOneCardTables.h>
#include<PktMaskOneSuitTables.h>
#include<PktMaskOneValTables.h>
#include<PktMaskTwoSuitTables.h>
#include<PktMaskTwoValTables.h>

// *** Constructors *** //

// *** Member Functions *** //

short ds::PktMask::remove(const Board& b) {
    short nRemoved = 0;
    for (Board::const_iterator it = Board.begin(); it != Board.end(); ++it) {
        const Card& c = *it;
        nRemoved += remove(c);
    }
    nRemaining_ -= nRemoved;
    return nRemoved;
}


short remove(const PktCards& pc) {
    short nRemoved = 0;
    const Card& ca(pc.first());
    const Card& cb(pc.second());
    
    bool wildValA(ca.hasWildValue());
    bool wildValB(cb.hasWildValue());
    bool wildSuitA(ca.hasWildSuit());
    bool wildSuitB(cb.hasWildSuit());
    const DeckInd diA(ca.deckIndex());
    const DeckInd diB(ca.deckIndex());
    
    #ifdef DSDEBUG
        if (ca.partsUnknown() || cb.partsUnknown()) {
            FatalError << "One of the parts has unknown components. Cards are:"
                << "\n    " << pc << std::endl;
            abort();
        }
        if (ca < cb) {
            FatalError << "Supplied PocketCards are unsorted. Cards are:\n"
                << "    " << pc << std::endl;
                
        }
    #endif
    if (!wildValA && !wildSuitA && !wildValB && !wildSuitB) {
        short maskIndex = deckIndicesHashTable_[PktDeckInd(diA, diB)];
        nRemoved = operator[](maskIndex);
        operator[](maskIndex) = 0;
    }
    else if (wildValA) {
        // implies wildValB (sorted requirement)
        // We know *? *?
        // 3 possibilities:
        //  1. oneSuit  ** *S or *S **
        //  2. twoSuit  *S *S 
        //  3. all wild ** **
        if (wildSuitA != wildSuitB) {
            // oneSuit tables
            const Suit s = ca.suit() + cb.suit() - Card::wildSuit;
            const VecDeckInd& myTable = oneSuitTables_[s];
            nRemoved += removeFromVeckDeckInd(myTable);
        } else if (!wildSuitA) {
            #ifdef DSDEBUG
                if (ca.suit() != cb.suit()) {
                    FatalError << "Unsuited wild values not allowed. Cards "
                        << "are:\n    " << pc << std::endl;
                }
            #endif
            // twoSuitTables
            const Suit s = ca.suit();
            const VecDeckInd& myTable = twoSuitTables_[s];
            nRemoved += removeFromVeckDeckInd(myTable);
        } else {
            // all wild
            // no need to set mask to zero, algorithms should keep count
            nRemoved = nRemaining_;
        }
    } else if (wildValB) {
        // We know V? *?
        // 2 possibilities:
        //  1. one card     VS **
        //  2. one value    V* **
        //  X. not allowed  V* *S, VS *S
        #ifdef DSDEBUG
        if (!wildSuitB) {
            // Algorithms should never deliver a suited B card in this case
            FatalError << "Unexpected wild combination. Cards are: \n"
                << "    " << pc << std::endl;
        }
        #endif
        // wildSuitB is implied
        if (wildSuitA) {
            // one value tables
            const BinCardVal bv = ca.binCardValue();
            const VecDeckInd& myTable = oneValueTables_[bv];
            nRemoved += removeFromVeckDeckInd(myTable);
        } else {
            // one card tables
            const VecDeckInd& myTable = oneCardTables_[diA];
            nRemoved += removeFromVeckDeckInd(myTable);
        }
    } else {
        // We know V? V?
        // One possibility:
        //  1. two vals         V* V*
        //  X. not allowed      VS V*, V* VS
        //  X. already checked  VS VS
        const VecDeckInd& myTable = twoValTables_[PktDeckInd(diA, diB)];
        nRemoved += removeFromVeckDeckInd(myTable);
    }
    
    nRemaining_ -= nRemoved;
    return nRemoved;
}

//- Remove any combinations with the given card
short remove(const Card& c);


short ds::PktMask::removeFromVecDeckInd(
    const VecDeckInd& table
) {
    short nRemoved = 0;
    for (
        VecDeckInd::const_iterator it = table.begin();
        it!= table.end();
        ++it
    ) {
        nRemoved += operator[](*it);
        operator[](*it) = 0;
    }
}


