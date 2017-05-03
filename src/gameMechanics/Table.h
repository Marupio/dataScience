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
            size_t 
            nSeats,
            const Blinds& blinds
        );
    
    
    // Public Member Functions

        // Access
        
            //- Return the board
            const Board& board() const;

            //- Return the GameManager
            const GameManager& manager() const;

            //- Get the current status
            statusEnum status() const;
            
            //- Get the current post-play action
            postPlayEnum postPlayAction() const;

            //- Set the post-play action
            void setPostPlayAction(postPlayEnum ppe);
            
            //- Get the number of seated players
            size_t nPlayers() const;

            //- Get the number of players leaving the table
            size_t nLeaving() const;

            //- Query current blinds
            const Blinds& blinds() const;
            

        // Actions        
            
            //- Start continuous play
            void playContinuous();

            //- Play one hand
            void playOnceThenPause();

            //- Play one hand then close table
            void playOnceThenDisband();

            //- Start, play based on ppAction
            void play();

            //- Sets ppAction to ppPause.
            //  If playing, finish current hand and stop
            void setTableToPause();

            //- Set ppAction to ppContinue
            //  If action is underway, it may end before ppAction is changed
            void setTableToContinuousPlay();

            //- Sets ppAction to ppDisband.
            //  If playing, finish current hand and disband.
            void setTableToDisband();

            //- Kicks all players from the table
            //  It is an error to call this while play is in action.
            //  ppAction must be ppDisband
            void disband();
            
            //- Change blinds
            void setBlinds(const Blinds& newBlinds);


private:

    // Private Member Functions

        //- Throws an error if the table is not ready for play
        void checkReadyForPlay() const;

        //- Advances dealer button to next valid seated player
        void moveDealerButton();

        //- Collects ante from every player that is not waiting for button
        void ante(Money amount);

        //- Collects the given blinds value from the given player
        void collectBlinds(PlayerRef& player, Money blinds);

        //- Deals cards to the active players
        void dealCards();

        //- Poll carded players if they wish to fast-fold
        //  Only does anything if game manager allows fast folds
        void checkForFastFolds(const SeatedPlayer& sp, Money totalBet)

        //- Collect bets
        //  Returns false if one player wins
        bool takeBets(SeatedPlayer player)

        //- Once all cards are on the board and the final betting is over,
        //  this function compares the remaining hands based on the pot they
        //  are vying for.  Once it determines who wins each pot, it
        //  distributes this information to the players via a showdown, giving
        //  losing players the chance to muck or show their hands.
        //  Upon completing this task, all players are given a chance to see
        //  the final result.
        void compareHands();

        //- Collects all pushed money into pots and clears players' pushed
        //  money
        void collectPushedMoney();
        
        //- Helper for collectBets, mostly for code reduction 
        void raiseHelper(
            Money raisedAmount,
            Money totalBet,
            Money& minRaise,
            SeatedPlayer& stopPlayer,
            const SeatedPlayer& player
        );

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
