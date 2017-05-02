// Virtual interface for a player at the table
// Manages stack size
#ifndef Player_h
#define Player_h

#include<vector.h>

// Forward declarations

class GameManager;
class Player;
class Table;

typedef Player* PlayerPtr;
typedef std::vector<PlayerPtr> VecPlayerPtr;


class Player {

public:

    // Friend classes
    friend class Table;


    // Public Data Types

        //- Summary structure contains everything other players can see
        struct Summary {
            size_t id;
            std::string name;
            Money stack;
            Money pushedMoney;
            Money rewardedMoney;
            bool allIn;
            bool hasCards;
            bool waitingForButton;
            bool sittingOut;
            bool disconnected;
            PktCards revealedPkt;
            
            // Constructors
            Summary(size_t idIn, const std::string& nameIn);
        };

        //- Player actions
        enum actionEnum {
            acUnknown,
            acFold,
            acCheck,
            acCall,
            acCallAllIn,
            acBet,
            acBetAllIn,
            acRaise,
            acRaiseAllIn
        };



    // Constructors
    
        //- Construct from components
        Player(const GameManager& gm, size_t id, const std::string& name);
    
    
    // Public Member Functions

        // Access
        
            //- Return entire summary structure
            const Summary& summary() const;
        
            //- Return player ID number
            size_t id() const;
        
            //- Return player name
            const std::string& name() const;

            //- Return stack size
            Money stack() const;        
            
            //- Return pushed money
            Money pushedMoney() const;
            
            //- Return rewarded money
            Money rewardedMoney() const;

            //- Return all-in flag
            bool allIn() const;

            //- True if the player has not yet folded
            bool hasCards() const;
        
            //- True if player cannot be dealt until has the dealer button
            bool waitingForButton() const;
            
            //- True if player is sitting out
            bool sittingOut() const;
            
            //- True if player is disconnected
            bool disconnected() const;

            //- Return revealedPkt
            const PktCards& revealedPocket() const;


        // Action
        
            //- Gives player the option to fastFold
            //  Defaults to false
            virtual bool fastFoldOption(Money totalBet);

            //- Take a bet
            //  Calls betOption and adds result to pushedMoney
            Money takeBet(Money totalBet, Money minRaise);

            //- Bet option for player interface
            //  Return additional pushed money:
            //      Call is (totalBet - summary_.pushedMoney)
            //      Min raise is (totalBet + minRaise - summary_.pushedMoney)
            virtual Money betOption(Money totalBet, Money minRaise);

            //- Option to reveal cards
            //  Copy any or all cards into revealedPkt if the player wants to
            virtual void revealWinningCardsOption();

            //- Option to reveal cards
            //  Copy any or all cards into revealedPkt if the player wants to
            virtual void revealLosingCardsOption();

            //- Called at the end of a hand of poker to allow players to take
            //  note of the results
            virtual void observeResults() void;

            //- Called at each action to allow players to take note of game
            //  play
            virtual void observeAction(
                const SeatedPlayer& player,
                actionEnum action,
                Money amount
            ) void;


protected:

    // Protected Member Functions
    
        //- Look at pocket
        const PktCards& pocket();


    // Protected Member Data
    
        //- Reference to the table
        const Table& table_;


private:

    // Private Member Functions

        // Forced actions - friend Table interface
        
            //- Set waitingForButton
            void setWaitingForButton(bool newValue);

            //- Collect ante or blinds
            //  Returns actual amount collected
            Money collect(Money amount);

            //- Deal cards to this player
            void dealPocket(VecDeckInd cardIndices);

            //- Show cards
            //  Copies pkt_ to revealedPkt_
            void showPocket();

            //- Reward player with a pot
            void reward(Money amount);

            //- Take back an overbet (when other players don't have that much)
            void returnExcess(Money amount);
            

            //- Remove pushed money
            void clearPushedMoney();
        

    // Private Member Data

        //- Reference to GameManager
        const GameManager& gm_;
    
        //- Player summary
        Summary summary_;
    
        //- Pocket cards
        PktCards pkt_;
};

#endif
