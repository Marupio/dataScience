#ifndef Table_h
#define Table_h

#include<atomic>
#include<Board.h>
#include<Blinds.h>
#include<Player.h>

// Forward declarations

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
            size_t nSeats,
            const Blinds& blinds,
            bool allowFastFolds,
            int dramaticPause
        );
    
    
    // Public Member Functions

        // Thread-unsafe
        // For use by play() thread, or after play() thread has joined
        
            //- Return the board
            const Board& board() const;

            //- Returns true if players are allowed to 'fast-fold' their hands
            bool allowFastFolds() const;
            
            //- Query current blinds
            const Blinds& blinds() const;

            //- Kicks all players from the table
            //  It is an error to call this while play is in action.
            //  ppAction must be ppDisband
            void disband();


        // Thread starters
        
            //- Start continuous play
            void playContinuous();

            //- Play one hand
            void playOnceThenPause();

            //- Play one hand then close table
            void playOnceThenDisband();

            //- Start, play based on ppAction
            void play();


        // Thread-safe interface

            // Access

                //- Get the current status
                statusEnum status() const;
                
                //- Get the current post-play action
                postPlayEnum postPlayAction() const;

                //- Set the post-play action
                void setPostPlayAction(postPlayEnum ppe);
                

            // Actions

                //- Sets ppAction to ppPause.
                //  If playing, finish current hand and stop
                void setTableToPause();

                //- Set ppAction to ppContinue
                //  If action is underway, it may end before ppAction is changed
                void setTableToContinuousPlay();

                //- Sets ppAction to ppDisband.
                //  If playing, finish current hand and disband.
                void setTableToDisband();
                
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

        //- If allInShowDown, pause for dramaticPause_ seconds
        void dramaticPause();
        
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
            const SeatedPlayer& player,
            size_t& nActiveNotAllIn
        );

        //- Have all carded players show their hands, activate allInShowDown
        //  flag
        void activateAllInShowDown();

        //- Tell all players about some event unrelated to any one player
        void shareEvent(Player::eventEnum event);

        //- Tell all players (except for player) about the action taken
        void shareAction(
            const SeatedPlayer& player,
            Player::actionEnum action,
            Money amount
        );

        //- Tell all players about the results of the last round
        void shareResults();

        //- Resets table for the next round
        void resetTable();


    // Private Data
    
        //- Deck
        Deck deck_;
    
        //- Board
        Board bd_;

        //- Position of the dealer button
        SeatedPlayer dealer_;
        
        //- Current blinds
        const Blinds* blinds_;
        
        //- Next blinds
        atomic<const Blinds*> nextBlinds_;
        
        //- When true, players can 'fast-fold' their hands and move to another
        //  table
        const bool allowFastFolds_;

        //- Number of seconds to pause between dealer actions during an all-in
        //  showdown. 0 or negative means no pause.
        const int dramaticPause_;
        
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
            
            //- Flag indicating that no more betting is necessary, it is an
            //  all-in show-down
            bool allInShowDown_;
};

#endif
