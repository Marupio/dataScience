#include<cctype>
#include<PktMask.h>

// Static data

#include<PktMaskDeckIndices.h>
#include<PktMaskDeckIndicesToPocketMask.h>
#include<PktMaskOneCardTables.h>
#include<PktMaskOneSuitTables.h>
#include<PktMaskOneValTables.h>
#include<PktMaskTwoSuitTables.h>
#include<PktMaskTwoValTables.h>

// *** Constructors *** //

// *** Member Functions *** //

short ds::PktMask::remove(const Board& b) {
    short nRemoved = 0;
    for (Board::const_iterator it = b.cbegin(); it != b.cend(); ++it) {
        const Card& c = *it;
        nRemoved += remove(c);
    }
    nRemaining_ -= nRemoved;
    return nRemoved;
}


short ds::PktMask::remove(const PktCards& pc) {
    short nRemoved = 0;
    const Card& ca(pc.first);
    const Card& cb(pc.second);
    
    bool wildValA(ca.hasWildValue());
    bool wildValB(cb.hasWildValue());
    bool wildSuitA(ca.hasWildSuit());
    bool wildSuitB(cb.hasWildSuit());
    
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
        const DeckInd diA(ca.deckIndex());
        const DeckInd diB(ca.deckIndex());
        short hashIndex;
        if (diA > diB) {
            hashIndex = diA + diB*51;
        } else {
            hashIndex = diB + diA*51;
        }
        short maskIndex = deckIndicesToPocketMask_[hashIndex];
        #ifdef DSDEBUG
        if (maskIndex < 0) {
            FatalError << "Attempting to remove cards with deck indices: ("
                << diA << " " << diB << "), but the mask index in not "
                << "positive." << std::endl;
            abort();
        }
        #endif
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
            nRemoved += removeFromVecDeckInd(myTable);
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
            nRemoved += removeFromVecDeckInd(myTable);
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
            const BinCardVal bv = ca.binValue();
            const VecDeckInd& myTable = oneValTables_[bv];
            nRemoved += removeFromVecDeckInd(myTable);
        } else {
            // one card tables
            const DeckInd diA(ca.deckIndex());
            const VecDeckInd& myTable = oneCardTables_[diA];
            nRemoved += removeFromVecDeckInd(myTable);
        }
    } else {
        // We know V? V?
        // One possibility:
        //  1. two vals         V* V*
        //  X. not allowed      VS V*, V* VS
        //  X. already checked  VS VS
        short index = (ca.value() - 2) + (cb.value() - 2)*12;
        const VecDeckInd& myTable = twoValTables_[index];
        #ifdef DSDEBUG
            FatalError << "Encountered empty entry in twoValTables with cards "
                << ca << ", " << cb << std::endl;
            abort();
        #endif
        nRemoved += removeFromVecDeckInd(myTable);
    }
    
    nRemaining_ -= nRemoved;
    return nRemoved;
}


short ds::PktMask::remove(const Card& c) {
    const DeckInd diA(c.deckIndex());
    const VecDeckInd& myTable = oneCardTables_[diA];
    return removeFromVecDeckInd(myTable);
}


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
    return nRemoved;
}

// ****** END ******
