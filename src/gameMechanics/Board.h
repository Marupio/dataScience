#ifndef Board_h
#define Board_h

#include<Card.h>
#include<VecValStats.h>

namespace ds {

class Board;
std::ostream& operator<<(std::ostream& os, const Board& c);
std::istream& operator>>(std::istream& in, Board& c);


class Board
{
public:

    // Public type data
    
        typedef VecCard::const_iterator const_iterator;
        typedef VecCard::const_reverse_iterator
            const_reverse_iterator;
        typedef VecCard::size_type size_type;
        typedef VecCard::difference_type difference_type;


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

        // Iterator access

            //- const_iterator begin
            const_iterator cbegin() const noexcept {
                return cards_.cbegin();
            }

            //- const_iterator end
            const_iterator cend() const noexcept {
                return cards_.cend();
            }

            //- const_reverse_iterator begin
            const_reverse_iterator crbegin() const noexcept {
                return cards_.crbegin();
            }

            //- const_iterator end
            const_reverse_iterator crend() const noexcept {
                return cards_.crend();
            }

            //- Return size
            size_type size() const noexcept {
                return cards_.size();
            }

        // Access
        
            //- Return cards on the board
            const VecCard& cards() const {
                return cards_;
            }

            //- Return stats of cards on the board
            const VecValStats& stats() const {
                return stats_;
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

            //- Has a value if board contains four of-a-kind
            CardVal foak() const {
                return foak_;
            }

            //- Has a value if board contains three of-a-kind, but not foak
            CardVal set() const {
                return set_;
            }

            //- Returns suit missing from three-of-a-kind
            Suit setMissingSuit() const {
                return setMissingSuit_;
            }

            //- If board has a pair, this will contains its value
            CardVal pairA() const {
                return pairA_;
            }
            
            //- Returns suits that are not represented in pairA
            const PktSuits& pairAMissingSuits() const {
                return pairAMissingSuits_;
            }

            //- If board has a second pair, this will contains its value
            CardVal pairB() const {
                return pairB_;
            }
            
            //- Returns suits that are not represented in pairB
            const PktSuits& pairBMissingSuits() const {
                return pairBMissingSuits_;
            }


        // Dealer interface

            //- Deliver flop
            void flop(const VecDeckInd& vd);
            
            //- Deliver turn
            void turn(DeckInd di);
            
            //- Deliver river
            void river(DeckInd di);
        
        
        // Derived searches

            //- Returns lowest value, not including avoids
            CardVal lowestValue() const;
            CardVal lowestValue(CardVal avoid);
            CardVal lowestValue(const PktVals& avoid);

            //- Returns two lowest card values, not including avoids
            PktVals lowestTwoValues() const;
            PktVals lowestTwoValues(CardVal avoid) const;
            PktVals lowestTwoValues(const PktVals& avoid) const;

            //- Returns high value, not including avoids
            CardVal highestValue() const;
            CardVal highestValue(CardVal avoid) const;
            CardVal highestValue(const PktVals& avoid) const;

            //- Returns two highest values, not including avoids
            PktVals highestTwoValues() const;
            PktVals highestTwoValues(CardVal avoid) const;
            PktVals highestTwoValues(const PktVals& avoid) const;

            //- Return the lowest flush card. Ignores the top three, because
            //  they are necessary for the flush.  i.e.:
            //      if (flushVals.size() == 3) return Card::lowAce;
            CardVal lowestFlushValue() const;
            PktVals lowestTwoFlushValues() const;

            //- Returns high value for flush, if available
            CardVal highestFlushValue() const;


    // Operators
    
    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const Board& c);
    friend std::istream& operator>>(std::istream& in, Board& c);


private:

    // Private member data

        //- Actual board cards
        VecCard cards_;

        // Data derived from cards_
        
            //- Contains sorted unique values, associated counts and suits.
            VecValStats stats_;

            //- Suit counts, indexed by suit
            SuitCount suitCounts_;

            //- If board has three suited cards, this is the suit, otherwise
            //  unknownSuit
            Suit flushSuit_;
            
            //- If one suit has 3 or more represented, this vector will contain
            //  their sorted values
            VecCardVal flushVals_;

            //- Has a value if board contains four of-a-kind
            CardVal foak_;

            //- Has a value if board contains three of-a-kind, but not foak
            CardVal set_;

            //- Suit missing from three-of-a-kind
            Suit setMissingSuit_;
            
            // Pair data

                //- If board contains one pair, its value will be in here
                CardVal pairA_;
                
                //- Shows suits that are NOT represented in pairA
                PktSuits pairAMissingSuits_;
                
                // If board contains a second pair, its value will be here
                CardVal pairB_;
                
                //- Shows suits that are NOT represented in pairB
                PktSuits pairBMissingSuits_;


    // Private member functions

        //- Reserve space in derived data
        void reserveSpace();
    
        //- Update derived data
        void updateDerivedData();
};

} // end namespace ds

#endif
