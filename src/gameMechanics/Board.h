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
            CardVal toak() const {
                return toak_;
            }

            //- Returns suit missing from three-of-a-kind
            Suit toakMissingSuit() const {
                return toakMissingSuit_;
            }

            //- If board has a pair, this will contains its value
            CardVal pairA() const {
                return pairA_;
            }
            
            //- Returns suits that are not represented in pairA
            const PktSuits& pairAMissingSuits() {
                pairAMissingSuits_
            }

            //- If board has a second pair, this will contains its value
            CardVal pairB() const {
                return pairB_;
            }
            
            //- Returns suits that are not represented in pairB
            const PktSuits& pairBMissingSuits() {
                pairBMissingSuits_
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
            CardVal toak_;

            //- Suit missing from three-of-a-kind
            Suit toakMissingSuit_;
            
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
