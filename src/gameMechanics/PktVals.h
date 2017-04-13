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

    // Static member data
    static const PktVals unknownValues;
    static const PktVals lowAces;
    static const PktVals highAces;


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
            bool has(CardVal cv) const;

            //- Returns true if pocket is a pair
            bool pairs() const;

            //- Returns true if pocket pair value is given value
            bool pairs(CardVal cv) const;


        // Edit
        
            //- Swap first and second
            void swap();

            //- Ensure first > second
            void sort();


    // Member operators
    
        //- True if both pocket cards are the same as the given, does not
        //  check wilds
        bool operator==(const PktVals& pc) const;

        //- False if either pocket cards differ from given, does not check
        //  wilds
        bool operator!=(const PktVals& pc);
        
        //- Cast to VecCardVal
        operator VecCardVal();

};


} // End namespace ds

#include<PktValsI.h>

#endif
