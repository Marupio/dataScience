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
            acBlinds,
            acBlindsAllIn,
            acFold,
            acCheck,
            acCall,
            acCallAllIn,
            acBetRaiseUnderAllIn,
            acBetRaise,
            acBetRaiseAllIn,
            acBetRaiseTwo,
            acBetRaiseTwoAllIn,
            acBetRaiseThree,
            acBetRaiseThreeAllIn,
            acBetRaiseFour,
            acBetRaiseFourAllIn
        };

    // Static Data
    
        static const std::array<std::string, 16> actionNames;


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
            virtual bool fastFoldOption(Money totalBet) = 0;

            //- Take a bet
            //  Calls betOption and adds result to pushedMoney
            Money takeBet(Money totalBet, Money minRaise);

            //- Bet option for player interface
            //  Return <action, amount>, which can be:
            //      <acUnknown, amount>:
            //          * action is determined based on amount
            //          * amount refers to extra money pushed
            //          * 0 is check (if enough was previously pushed) or fold
            //              (if insufficient was pushed)
            //          * bounds checked amount based on stack size (for
            //              all-in), totalBet (to meet call), and minRaise.
            //      <action, negativeAmount>:
            //          * action is what the player wants to do
            //          * actual amount is determined based on this request
            //          * options are:
            //              - acFold (goes to 'check' if pushedMoney==totalBet)
            //              - acCheck (only valid if pushedMoney==totalBet)
            //              - acCall (goes to 'check' if pushedMoney==totalBet)
            //              - acRaise (min raise)
            //              - acRaiseTwo (2*minRaise)
            //              - acRaiseThree (3*minRaise)
            //              - acRaiseFour (4*minRaise)
            //              - acRaiseAllIn
            //              - invalid action goes to check/fold
            //      <expectedAction, amount>:
            //          * action is taken as what the player intends to do
            //          * actual action is determined by amount, similar to
            //              the <acUnknown, amount> case
            //          * if actual action deviates from intended action, a
            //              warning is posted.
            //  In all cases, amount is the call / raise amount, i.e. extra
            //  pushed money, not the total bet
            virtual std::pair<actionEnum, Money> betOption(
                Money totalBet,
                Money minRaise
            ) = 0;

            //- Give player a call or fold option
            //  Calls callFoldOption and adds result to pushedMoney
            Money takeCall(Money totalBet);

            //- Player can only call or fold
            //  Situation arises with an under-raise all-in, and the player has
            //  acted already.
            //  Return <action, amount>, which can be:
            //      <acUnknown, amount>:
            //          * action is determined based on amount
            //      <action, negativeAmount>:
            //          * amount is determined based on action
            //          * valid actions are:
            //              - acCall
            //              - acFold
            //      <expectedAction, amount>:
            //          * action is determined based on amount
            //          * expectedAction is provided by player for verification
            //  In all cases, amount is the call / raise amount, i.e. extra
            //  pushed money, not the total bet
            virtual std::pair<actionEnum, Money> callFoldOption(
                Money totalBet
            ) = 0;

            //- Calls revealWinningCardsOption(), and copies cards into
            //  revealedPkt as requested
            void askRevealWinningCards();

            //- Option to reveal cards
            //  Returns:
            //     -1 do not reveal any
            //      0 reveal Pkt.first
            //      1 reveal Pkt.second
            //      2 reveal both
            //  Defaults to -1
            virtual int revealWinningCardsOption();

            //- Calls revealLosingCardsOption(), and copies cards into
            //  revealedPkt as requested
            void askRevealLosingCards();

            //- Option to reveal cards
            //  Returns:
            //     -1 do not reveal any
            //      0 reveal Pkt.first
            //      1 reveal Pkt.second
            //      2 reveal both
            //  Defaults to -1
            virtual void revealLosingCardsOption();

            //- Called at the end of a hand of poker to allow players to take
            //  note of the results
            virtual void observeResults() void = 0;

            //- Called at each action to allow players to take note of game
            //  play
            virtual void observeAction(
                const SeatedPlayer& player,
                actionEnum action,
                Money amount
            ) void = 0;


protected:

    // Protected Member Functions
    
        //- Look at pocket
        const PktCards& pocket() const;


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


        // Internal functions
        
            //- Interpret result of reveal[Winning|Losing]CardsOption()
            void parseRevealOption(int answer);

        

    // Private Member Data

        //- Reference to GameManager
        const GameManager& gm_;
    
        //- Player summary
        Summary summary_;
    
        //- Pocket cards
        PktCards pkt_;
};

#endif
