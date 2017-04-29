#ifndef Table_h
#define Table_h

#include<atomic>
#include<Board.h>
#include<Blinds.h>
#include<Player.h>

// Forward declarations

class GameManager;

class Table:
    public Seats
{

public:

    // Public Data Types
    
        //- Play status
        enum statusEnum {
            seUnknown,
            seEmpty,
            sePaused,
            sePreFlop,
            seFlop,
            seTurn,
            seRiver
        };
        
        //- Post play actions
        enum postPlayEnum {
            ppContinue,
            ppPause,
            ppDisband
        };


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
            

        // Actions        
            
            //- Start
            void play();

            //- Finish current hand and halt play until released
            void pause();

            //- Set ppAction to ppContinue
            void resume();
            
            //- Finish current hand and remove all players from the table,
            //  appending their id to leaving_
            void disband();

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

        //- Position of the dealer button
        SeatedPlayer dealer_;
        
        //- Current blinds
        const Blinds* blinds_;
        
        //- Next blinds
        const Blinds* nextBlinds_;
        
        //- Current status
        atomic<statusEnum> status_;

        //- Post play action
        atomic<postPlayEnum> ppAction_;

        
        // Data generated during play
        
            //- Money the players have pushed forward in this round
            VecPushedMoney pushedMoney_;

            //- Pot structure
            VecPot pots_;
            
            //- First to show hand
            SeatedPlayer firstToShow_;
};

#endif
