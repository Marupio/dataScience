#ifndef ValStats_h
#define ValStats_h

#include <tuple>
#include <Card.h>

// Contains meta data about a single card value within a set of cards
// Includes the number of cards represented and the suits represented

namespace ds {

// Forward declarations
class ValStats;
std::ostream& operator<<(std::ostream& os, const ValStats& vs);
std::istream& operator>>(std::istream& in, ValStats& vs);

class ValStats: 
    public std::tuple<CardVal, short, SuitCount>
{
    // Data contained in base:
    // <0> CardVal:     card value
    // <1> short:       number of cards with this card value
    // <2> SuitCount:   suits represented

public:

    // Constructors
    
        //- Construct null
        ValStats();

        //- Construct from components
        ValStats(CardVal value, short nCards, SuitCount& suits);

    //- Destructor
    ~ValStats() {}

    // Member functions
    
        // Access
        
            //- Access the card value
            CardVal& value();
            
            //- Return the card value
            CardVal value() const;
            
            //- Access the card count
            short& nCards();

            //- Return the card count
            short nCards() const;
            
            //- Access the suit count
            SuitCount& suits();

            //- Access the suit count
            const SuitCount& suits() const;


    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const Card& c);
    friend std::istream& operator>>(std::istream& in, Card& c);

};

} // end namespace ds

#endif
