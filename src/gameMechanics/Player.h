// Virtual interface for a player at the table
// Manages stack size
#ifndef Player_h
#define Player_h

#include<vector.h>

// Forward declarations

class Player;

typedef Player* PlayerPtr;
typedef std::vector<PlayerPtr> VecPlayerPtr;

class Player {

public:

    // Constructors
    
        //- Construct from components
        Player(
            const GameManager& gm,
            size_t nSeats,
            const Blinds& blinds
        );
    
    
    // Public Member Functions

        // Forced actions

            //- Set waitingForButton
            void setWaitingForButton(bool newValue);

            //- Collect ante or blinds
            //  Returns actual amount collected
            Money collect(Money amount);

            //- Deal cards to this player
            void dealPocket(VecDeckInd cardIndices);
            

        // Access

            //- True if player cannot be dealt until has the dealer button
            bool waitingForButton() const;

            //- True if the player has not yet folded
            bool hasCards() const;
        
            //- Return stack size
            Money stack() const;        
            
            //- Return all-in flag
            bool allIn() const;


        // Action
        
            //- Gives player the option to fastFold
            bool fastFoldOption(Money totalBet);


protected:

    // Protected Member Functions
    
        //- Look at pocket
        const PktCards& pocket();


private:

    // Private Member Data
    
        //- Stack size
        Money stack_;

        //- Waiting for button flag
        bool waitingForButton_;

        //- All-in flag
        bool allIn_;

        //- Pocket cards
        PktCards pkt_;
};

#endif
