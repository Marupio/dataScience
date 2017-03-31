#ifndef Board_h
#define Board_h

#include<Card.h>

namespace ds {

class Board;
std::ostream& operator<<(std::ostream& os, const Board& c);
std::istream& operator>>(std::istream& in, Board& c);

class Dealer;

class Board
{
public:

    friend class Dealer;

    // Static data

    static const short maxCardsOnBoard_;
    static const short flopSize_;
    static const short sizeBeforeRiver_;

    // Constructors

        //- Construct null    
        Board();

        //- Construct from list of deck indices
        Board(VecDeckInd di);
        
        //- Construct from istream
        Board(std::istream& is);
    

    //- Destructor
    ~Board() {}


    // Member functions

        // Access
        
            //- Return cards on the board
            const VecCard& cards() const {
                return cards_;
            }

            //- Number of cards on the board
            VecCard::size_type size() const {
                return cards_.size();
            }

            //- Return sorted unique values
            const VecCardVal& sortedUniqueVals() const {
                return sortedUniqueVals_;
            }
            
            //- Return counts of each value
            const std::vector<short>& sortedUniqueValCounts() const {
                return sortedUniqueValCounts_;
            }

            //- Return suit counts
            const std::vector<short>& suitCounts() const {
                return suitCounts_;
            }

            //- Return flush suit
            Suit flushSuit() const {
                return flushSuit_;
            }

            //- Return sorted flush values, non-empty if 3 suited are on the
            //  board
            const VecCardVal& sortedFlushVals() const {
                return sortedFlushVals_;
            }
        

    // Operators
    
    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const Board& c);
    friend std::istream& operator>>(std::istream& in, Board& c);


private:

    // Private member data

        //- Actual board cards
        VecCard cards_;

        // Data derived from cards_
        
            //- Values on board, sorted lowest to highest with duplicates
            //  removed
            VecCardVal sortedUniqueVals_;
            
            //- Indexed by sortedUniqueValues, gives the number of occurrences
            //  of each value
            std::vector<short> sortedUniqueValCounts_;

            //- Suit counts, indexed by suit
            std::vector<short> suitCounts_;

            //- If board has three suited cards, this is the suit, otherwise
            //  unknownSuit
            Suit flushSuit_;
            
            //- If one suit has 3 or more represented, this vector will contain
            //  their sorted values
            VecCardVal sortedFlushVals_;


    // Private member functions

        //- Dealer interface
public:
            //- Deliver flop
            void flop(const VecDeckInd& vd);
            
            //- Deliver turn
            void turn(DeckInd di);
            
            //- Deliver river
            void river(DeckInd di);
private:
        //- Reserve space in derived data
        void reserveSpace();
    
        //- Update derived data
        void updateDerivedData();
};

} // end namespace ds

//#include<BoardI.h>

#endif
