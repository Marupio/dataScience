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
            const VecCardVal& values() const {
                return values_;
            }
            
            //- Return counts of each value
            const std::vector<short>& valueCounts() const {
                return valueCounts_;
            }

            //- Return suits of sorted unique values
            const VecVecSuit& valueSuits() const {
                return valueSuits_;
            }

            //- Return suit counts
            const SuitCount& suitCounts() const {
                return suitCounts_;
            }

            //- Return flush suit
            Suit flushSuit() const {
                return flushSuit_;
            }

            //- Return sorted flush values, non-empty if 3 suited are on the
            //  board
            const VecCardVal& flushVals() const {
                return flushVals_;
            }

            //- Has a value if board contains four of a kind
            CardVal foak() const {
                return foak_;
            }

            //- Has a value if board contains three of a kind, but not foak
            CardVal toak() const {
                return toak_;
            }
            
            //- If board contains one pair, first will have the value,
            //  if board contains two pairs, second will have lower value
            const PktVals& pairs() const {
                return pairs_;
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
            VecCardVal values_;
            
            //- Indexed by values, gives the number of occurrences
            //  of each value
            std::vector<short> valueCounts_;

            //- Sorted unique value suits
            //      valueSuits[index of values_][i] =
            //      suit of the ith duplicate value.  For pairs, i = 0..1
            VecVecSuit valueSuits_;

            //- Suit counts, indexed by suit
            SuitCount suitCounts_;

            //- If board has three suited cards, this is the suit, otherwise
            //  unknownSuit
            Suit flushSuit_;
            
            //- If one suit has 3 or more represented, this vector will contain
            //  their sorted values
            VecCardVal flushVals_;

            //- Has a value if board contains four of a kind
            CardVal foak_;

            //- Has a value if board contains three of a kind, but not foak
            CardVal toak_;
            
            //- If board contains one pair, first will have the value,
            //  if board contains two pairs, second will have lower value
            PktVals pairs_;


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
