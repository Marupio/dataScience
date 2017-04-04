#include<Card.h>

// Contains meta data about a single card value within a set of cards
// Includes the number of cards represented and the suits represented

// Forward declarations
//class ValStats;

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
        ValStats():
            std::tuple<CardVal, short, SuitCount>(
                Card::unknownValue,
                0,
                {0, 0, 0, 0}
            ) {}

        //- Construct from components
        ValStats(CardVal value, short nCards, SuitCount& suits):
            std::tuple<CardVal, short, SuitCount>(
                value,
                nCards,
                suits
            ) {}

    //- Destructor
    ~ValStats() {}

    // Member functions
    
        // Access
        
            //- Access the card value
            CardVal& value() {
                return get<0>;
            }
            
            //- Return the card value
            CardVal value() const {
                return get<0>;
            }
            
            //- Access the card count
            short& valueCount() {
                return get<1>;
            }

            //- Return the card count
            short nCards() const {
                return get<1>;
            }
            
            //- Access the suit count
            SuitCount& suits() {
                return get<2>;
            }

            //- Access the suit count
            const SuitCount& suits() const {
                return get<2>;
            }
};
