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
        // All these functions return the amount actually provided, which can
        // be less with all-in situations

            //- Collect ante or blinds
            Money collect(Money amount);


        // Access

            //- True if player cannot be dealt until has the dealer button
            bool waitingForButton() const;

            //- True if the player has not yet folded
            bool hasCards() const;
        
            //- Return stack size
            Money stack() const;        
            
            //- Return all-in flag
            bool allIn() const;

            

private:

    // Private Member Data
    
        //- Stack size
        Money stack_;

        //- Waiting for button flag
        bool waitingForButton_;

        //- All-in flag
        bool allIn_;


        // Actions        
            
            //- Start / resume play
            void play();

            //- Finish current hand and halt play until released
            void pause();
            
            //- Finish current hand and remove all players from the Player
            void disband();

            //- Add player
            void addPlayer(PlayerRef& player);
            
            //- Add players
            void addPlayers(VecPlayerRef& players);
        
            //- Access leavingPlayer buffer
            VecPlayerRef& leavingPlayer();

            //- Change blinds
            void setBlinds(const Blinds& newBlinds);


private:

    // Private Member Functions

        //- Throws an error if the Player is not ready for play
        void ds::Player::checkReadyForPlay() const;


    // Private Data
    
        //- Reference to the game manager
        const GameManager& gm_;
    
        //- Deck
        Deck deck_;
    
        //- Board
        Board bd_;

        //- Number of seats available
        const size_t nSeats_;

        //- Players seated at the Player
        VecPlayerRef seated_;

        //- Position of the dealer button, indexed according to seated_
        VecPlayerRef::iterator dealer_;
        
        //- Number of players seated - this is derived data that must be kept
        //  up to date
        size_t nSeated_;
        
        //- Players leaving the Player
        VecPlayerRef leaving_;

        //- Current blinds
        const Blinds* blinds_;
        
        //- Next blinds
        const Blinds* nextBlinds_;
        
        //- Current status
        statusEnum status_;
};

#endif
