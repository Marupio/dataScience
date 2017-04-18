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
        static const char HtUnknown;          // 0
        static const char HtHighCard;         // 1
        static const char HtPair;             // 2
        static const char HtTwoPair;          // 3
        static const char HtSet;              // 4
        static const char HtStraight;         // 5
        static const char HtFlush;            // 6
        static const char HtFullHouse;        // 7
        static const char HtFoak;             // 8
        static const char HtStraightFlush;    // 9

        //- Names associated with each Ht char
        static const std::array<std::string, 10> HandTypeNames;


        // Constructors    

            //- Construct from components
            HandType(char t, PktVals v);

            //- Construct from components
            HandType(char t, CardVal vA, CardVal vB);


        // Public Member Functions
        
            // Access
            
                //- Return hand type enumeration
                char ht() const;

                //- Return values
                const PktVals& values() const;



    // Private data
    
            //- Type of hand (e.g. two-pair, high card, etc)
            char ht_;
            
            //- Card value associated with the hand type. Examples:
            //  * Straight - this is the highest card of the straight
            //  * Full house - this is the value of the set
            //  * Pair - this is the value of the pair
            PktVals values_;

}
#endif
