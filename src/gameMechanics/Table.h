#ifndef Table_h
#define Table_h

#include<atomic>
#include<Board.h>
#include<Blinds.h>
#include<PlayerRef.h>

// Forward declarations

class GameManager;

class Table {

public:

    // Public Static Data
    
    static atomic<short> seUnknown = 0;
    static atomic<short> seEmpty = 1;
    static atomic<short> sePaused = 2;
    static atomic<short> sePreFlop = 3;
    static atomic<short> seFlop = 4;
    static atomic<short> seTurn = 5;
    static atomic<short> seRiver = 6;


    // Constructors
    
        //- Construct from components
        Table(
            const GameManager& gm,
            size_t nSeats,
            const Blinds& blinds
        );
    
    
    // Public Member Functions

        // Access
        
            //- Return the board
            const Board& board() const;

            //- Return the GameManager
            const GameManager& manager() const;

            //- Return the number of seats
            size_t nSeats() const;

            //- Get the current status
            atomic<short> status() const;
            
            //- Get the number of active players
            size_t nPlayers() const;

            //- Get the number of players leaving the table
            size_t nLeaving() const;

            //- Query current blinds
            const Blinds& blinds() const;

            //- Return all seated players
            const VecPlayerRef& players() const;
            

        // Actions        
            
            //- Start / resume play
            void play();

            //- Finish current hand and halt play until released
            void pause();
            
            //- Finish current hand and remove all players from the table
            void disband();

            //- Add player
            void addPlayer(PlayerRef& player);
            
            //- Add players
            void addPlayers(VecPlayerRef& players);
        
            //- Access leavingTable buffer
            VecPlayerRef& leavingTable();

            //- Change blinds
            void setBlinds(const Blinds& newBlinds);


private:

    // Private Member Functions

        //- Throws an error if the table is not ready for play
        void ds::Table::checkReadyForPlay() const;


    // Private Data
    
        //- Reference to the game manager
        const GameManager& gm_;
    
        //- Deck
        Deck deck_;
    
        //- Board
        Board bd_;

        //- Number of seats available
        const size_t nSeats_;

        //- Players seated at the table
        VecPlayerRef seated_;

        //- Position of the dealer button, indexed according to seated_
        VecPlayerRef::iterator dealer_;
        
        //- Number of players seated - this is derived data that must be kept
        //  up to date
        size_t nSeated_;
        
        //- Players leaving the table
        VecPlayerRef leaving_;

        //- Current blinds
        const Blinds* blinds_;
        
        //- Next blinds
        const Blinds* nextBlinds_;
        
        //- Current status
        statusEnum status_;
};

#endif
