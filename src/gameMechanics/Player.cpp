#include<Player.h>
#include<Table.h>
#include<GameManager.h>

// ****** Constructors ****** //

ds::Player::Summary::Summary(size_t idIn, const std::string& nameIn):
    id(idIn),
    name(nameIn),
    stack(0),
    pushedMoney(0),
    rewardedMoney(0),
    allIn(false),
    hasCards(false),
    waitingForButton(false),
    sittingOut(false),
    disconnected(false)
{}


ds::Player::Player(const GameManager& gm, size_t id, const std::string& name):
    gm_(gm),
    summary_(id, name)
{}
    

// ****** Public Member Functions ****** //

const ds::Player::Summary& ds::Player::summary() const {
    return summary_;
}
        

size_t ds::Player::id() const {
    return summary_.id;
}
        

const std::string& name() const {
    return summary_.name;
}


ds::Money ds::Player::stack() const {
    return summary_.stack;
}


ds::Money ds::Player::pushedMoney() const {
    return summary_.pushedMoney;
}


ds::Money ds::Player::rewardedMoney() const {
    return summary_.rewardedMoney();
}


bool ds::Player::allIn() const {
    return summary_.allIn;
}


bool ds::Player::hasCards() const {
    return summary_.hasCards;
}


bool ds::Player::waitingForButton() const {
    return summary_.waitingForButton;
}


bool ds::Player::sittingOut() const {
    return summary_.sittingOut;
}


bool ds::Player::disconnected() const {
    return summary_.disconnected;
}


const ds::PktCards& ds::Player::revealedPocket() const {
    return summary_.revealedPocket;
}


bool fastFoldOption(Money totalBet) {
    return false;
}


            //  Calls betOption and adds result to pushedMoney
ds::Money ds::Player::takeBet(Money totalBet, Money minRaise) {
    std::pair<actionEnum, Money> actionPush = betOption(totalBet, minRaise);
    actionEnum action = actionPush.first;
    Money newlyPushed = actionPush.second;
    if (newlyPushed < 0) {
        // Player interacts based on actionEnum
    } else {
        // Player interacts based on newlyPushed
        actionEnum expectedAction = acUnknown;
        if (newlyPushed < 0) {
            Warning << "Player '" << summary_.name << "', id " << summary_.id
                << " attempting to bet negative amount (" << newlyPushed << ")."
                << std::endl;
            newlyPushed = 0;
        }
        if (newlyPushed < SMALL && summary_.pushedMoney < totalBet) {
            // Folded
            expectedAction = acFold;
            summary_.hasCards = false;
            pkt_ = PktCards(Card::unknownCard, Card::unknownCard);
        } else if (newlyPushed > summary_.stack) {
            Warning << "Player '" << summary_.name << "', id " << summary_.id
                << " attempting to bet " << newlyPushed << " when stack is "
                << summary_.stack << ". Bet reset to stack size and player is "
                << "now all-in." << std::endl;
                newlyPushed_ = summary_.stack;
        }
        if (newlyPushed - summary_.stack < SMALL) {
            // Player is all-in
            summary_.allIn = true;
            expectedAction = 
        } else if (summary_.pushedMoney + newlyPushed + SMALL < totalBet) {
            // Below call limit
            Warning << "Player '" << summary_.name << "', id " << summary_.id
                << " attempting to call with "
                << Money(summary_.pushedMoney + newlyPushed) << " from a stack "
                << "size of " << summary_.stack << ", when required call is "
                << totalBet << ".  Call adjusted to correct amount."
                << std::endl;
            newlyPushed = totalBet - summary_.pushedMoney;
        } else if 
        }

        summary_.stack -= newlyPushed;
        summary_.pushedMoney += newlyPushed;
    }
}

            //- Bet option for player interface
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
    
        //- Player summary
        Summary summary_;
    
        //- Pocket cards
        PktCards pkt_;
};

#endif
