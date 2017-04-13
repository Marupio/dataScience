#ifndef PktVals_h
#define PktVals_h

#include<types.h>

namespace ds {

// Forward declarations
class Card;
class PktVals;

typedef std::vector<PktVals> VecPktVals;

class PktVals
:
    public std::pair<CardVal, CardVal>
{
public:


    // Constructors

        //- Construct null
        PktVals();

        //- Construct from components
        PktVals(const CardVal& valA, const CardVal& valB);

        //- Construct from cards, stripping the suit
        PktVals(const Card& CardA, const Card& CardB);
        
        //- Construct from deck indices
        PktVals(DeckInd diA, DeckInd diB);

        //- Construct from VecDeckInd
        PktVals(VecDeckInd vdi);

        //- Construct from istream
        PktVals(std::istream& is);


    //- Destructor
    ~PktVals() {}

    
    // Member functions
    
        // Tests
        
            //- Returns true if pocket has given value
            inline bool has(CardVal cv) const;

            //- Returns true if pocket is a pair
            inline bool pairs() const;

            //- Returns true if pocket pair value is given value
            inline bool pairs(CardVal cv) const;


        // Edit
        
            //- Swap first and second
            void swap();


    // Member operators
    
        //- True if both pocket cards are the same as the given
        inline bool operator==(const PktVals& pc) const;

        //- False if either pocket cards differ from given
        inline bool operator!=(const PktVals& pc);
        
        //- Cast to VecCardVal
        inline operator VecCardVal();

};


} // End namespace ds

#include<PktValsI.h>

#endif
