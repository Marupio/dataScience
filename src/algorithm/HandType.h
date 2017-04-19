#ifndef HandType_h
#define HandType_h

#include<string>
#include<types.h>
#include<Board.h>
#include<PktCards.h>
#include<StraightCompleters.h>

namespace ds {

class HandType
{

public:

    // Public static data

        //- Hand types
        static const char HtUnknown       = 0;
        static const char HtHighCard      = 1;
        static const char HtPair          = 2;
        static const char HtTwoPair       = 3;
        static const char HtSet           = 4;
        static const char HtStraight      = 5;
        static const char HtFlush         = 6;
        static const char HtFullHouse     = 7;
        static const char HtFoak          = 8;
        static const char HtStraightFlush = 9;

        //- Names associated with each Ht char
        static const std::array<std::string, 10> HandTypeNames;


        // Constructors    

            //- Construct given board and pocket
            HandType(const Board& bd, const PktCards& pkt);


        // Public Member Functions
        
            // Access
            
                //- Return hand type enumeration
                char ht() const {
                    return ht_;
                }

                //- Return values
                const PktVals& values() const {
                    return values_;
                }


    // Private member functions
    
        //- Determine hand type stats
        void getHandType(const Board& bd, const PktCards& pkt);
        
        //- Set all private data
        void setHandType(char t, PktVals v);
        void setHandType(char t, CardVal vA, CardVal vB);
    

    // Private data
    
            //- Type of hand (e.g. two-pair, high card, etc)
            char ht_;
            
            //- Card value associated with the hand type. Examples:
            //  * Straight - this is the highest card of the straight
            //  * Full house - this is the value of the set
            //  * Pair - this is the value of the pair
            PktVals values_;

};


} // End namespace ds

#endif

