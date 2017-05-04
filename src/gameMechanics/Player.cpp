#include<cmath>
#include<Player.h>
#include<Table.h>
#include<GameManager.h>

// ****** Static Data Members ****** //

const std::array<std::string, 16> ds::Player::actionNames = {
    "Unknown",
    "Blinds",
    "BlindsAllIn",
    "Fold",
    "Check",
    "Call",
    "CallAllIn",
    "BetRaiseUnderAllIn",
    "BetRaise",
    "BetRaiseAllIn",
    "BetRaiseTwo",
    "BetRaiseTwoAllIn",
    "BetRaiseThree",
    "BetRaiseThreeAllIn",
    "BetRaiseFour",
    "BetRaiseFourAllIn"
};

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
    if (summary_.allIn) {
        return 0;
    }

    std::pair<actionEnum, Money> actionPush = betOption(totalBet, minRaise);
    actionEnum action = actionPush.first;
    Money newlyPushed = actionPush.second;

    if (newlyPushed < 0) {
        // Player interacts based on actionEnum
        switch (action) {
        acFold: {
            if (std::abs(summary_.pushedMoney - totalBet) < SMALL) {
                Warning << "Player '" << summary_.name << "', id "
                    << summary_.id << " chose 'Fold' when pushedMoney ("
                    << summary_.pushedMoney << ") was was equal to totalBet ("
                    << totalBet << "). Hand is checked." << std::endl;
                newlyPushed = 0;
                break;
            }
            // Fold okay
            newlyPushed = 0;
            summary_.hasCards = false;
            pkt_ = PktCards(Card::unknownCard, Card::unknownCard);
            break;
        }
        acCheck: {
            if (std::abs(summary_.pushedMoney - totalBet) > SMALL) {
                Warning << "Player '" << summary_.name << "', id "
                    << summary_.id << " chose 'Check' when pushedMoney ("
                    << summary_.pushedMoney << ") was less than totalBet ("
                    << totalBet << "). Hand is folded." << std::endl;
            }
            newlyPushed = 0;
            summary_.hasCards = false;
            pkt_ = PktCards(Card::unknownCard, Card::unknownCard);
            break;
        }
        acCall: {
            newlyPushed = totalBet - summary_.pushedMoney;
            // check for all in
            if (summary_.stack - newlyPushed < SMALL) {
                newlyPushed = summary_.stack;
                summary_.allIn = true;
            }
            break;
        }
        acBetRaise: {
            newlyPushed = totalBet + minRaise - summary_.pushedMoney;
            // check for all in
            if (summary_.stack - newlyPushed < SMALL) {
                newlyPushed = summary_.stack;
                summary_.allIn = true;
            }
            break;
        }
        acBetRaiseTwo: {
            newlyPushed = totalBet + 2*minRaise - summary_.pushedMoney;
            // check for all in
            if (summary_.stack - newlyPushed < SMALL) {
                newlyPushed = summary_.stack;
                summary_.allIn = true;
            }
            break;
        }
        acBetRaiseThree: {
            newlyPushed = totalBet + 3*minRaise - summary_.pushedMoney;
            // check for all in
            if (summary_.stack - newlyPushed < SMALL) {
                newlyPushed = summary_.stack;
                summary_.allIn = true;
            }
            break;
        }
        acBetRaiseFour: {
            newlyPushed = totalBet + 4*minRaise - summary_.pushedMoney;
            // check for all in
            if (summary_.stack - newlyPushed < SMALL) {
                newlyPushed = summary_.stack;
                summary_.allIn = true;
            }
            break;
        }
        acBetRaiseAllIn: {
            newlyPushed = summary_.stack;
            summary_.allIn = true;
            break;
        }
        default: {
            Warning << "Player '" << summary_.name << "', id "
                << summary_.id << " chose '" << actionNames[action] << "', "
                << "which is not a valid action-based choice. Hand is checked/"
                << "folded." << std::endl;
            if (std::abs(summary_.pushedMoney - totalBet) > SMALL) {
                summary_.hasCards = false;
                pkt_ = PktCards(Card::unknownCard, Card::unknownCard);
            }
            newlyPushed = 0;
            break;
        } // end default
        } // end switch
    } else {
        // Player interacts based on newlyPushed
        actionEnum expectedAction = acUnknown;

        // Bounds check
        if (newlyPushed > summary_.stack) {
            Warning << "Player '" << summary_.name << "', id " << summary_.id
                << " attempting to bet " << newlyPushed << " when stack is "
                << summary_.stack << ". Bet reset to stack size and player is "
                << "now all-in." << std::endl;
                newlyPushed_ = summary_.stack;
        }

        // Check for all-in
        if (newlyPushed - summary_.stack < SMALL) {
            summary_.allIn = true;
        }
        
        Money totalPushedBet = newlyPushed + summary_.pushedMoney;
        Money raised = totalPushedBet - totalBet;

        // Check for fold / check
        if (newlyPushed < SMALL) {
            if (summary_.pushedMoney + SMALL < totalBet) {
                // Folded
                expectedAction = acFold;
                summary_.hasCards = false;
                pkt_ = PktCards(Card::unknownCard, Card::unknownCard);
            } else {
                // Check
                expectedAction = acCheck;
            }
        } else // check for under-call
        if (totalPushedBet + SMALL < totalBet) {
            // insufficient for call
            if (summary_.allIn) {
                expectedAction = acCallAllIn;
            } else {
                Warning << "Player '" << summary_.name << "', id " << summary_.id
                    << " attempting to call with " << totalPushedBet << " from "
                    << "a stack size of " << summary_.stack << ", when "
                    << "required call is " << totalBet << ".  Hand is folded."
                    << std::endl;
                expectedAction = acFold;
                summary_.hasCards = false;
                pkt_ = PktCards(Card::unknownCard, Card::unknownCard);
                newlyPushed = 0;
            }
        } else // check for call
        if (totalPushedBet - totalBet < SMALL) {
            expectedAction = acCall;
        } else // check for under-raise
        if (raised + SMALL < minRaise) {
            if (summary_.allIn) {
                expectedAction = acRaiseUnderAllIn;
            } else {
                Warning << "Player '" << summary_.name << "', id "
                    << summary_.id << " attempting to raise with " << raised
                    << " from a stack size of " << summary_.stack << ", when "
                    << "min raise is " << minRaise << ".  Player calls instead."
                    << std::endl;
                newlyPushed = totalBet - summary_.pushedMoney;
                expectedAction = acCall;
            }
        } else // check for raiseOne
        if (raised + SMALL < 2*minRaise) {
            if (summary_.allIn) {
                expectedAction = acBetRaiseAllIn;
            } else {
                expectedAction = acBetRaise;
            }
        } else // check for raiseTwo
        if (raised + SMALL < 3*minRaise) {
            if (summary_.allIn) {
                expectedAction = acBetRaiseTwoAllIn;
            } else {
                expectedAction = acBetRaiseTwo;
            }
        }
        } else // check for raiseThree
        if (raised + SMALL < 4*minRaise) {
            if (summary_.allIn) {
                expectedAction = acBetRaiseThreeAllIn;
            } else {
                expectedAction = acBetRaiseThree;
            }
        } else {
            // All else is raiseFour
            if (summary_.allIn) {
                expectedAction = acBetRaiseFourAllIn;
            } else {
                expectedAction = acBetRaiseFour;
            }
        }

        // These variables are now out of date
        totalPushedBet = 0;
        raised = 0;

        if (action != acUnknown) {
            // Player wants to double-check action
            if (action != expectedAction) {
                // Allow player to miss 'all-in' aspect
                bool actionOkay = false;
                switch (action) {
                case acCall: {
                    if (expectedAction == acCallAllIn) {
                        actionOkay = true;
                    }
                    break;
                }
                case acBetRaise: {
                    if (
                        expectedAction == acBetRaiseAllIn
                     || expectedAction == acBetRaiseUnderAllIn
                    ) {
                        actionOkay = true;
                    }
                    break;
                }
                case acBetRaiseTwo: {
                    if (expectedAction == acBetRaiseTwoAllIn) {
                        actionOkay = true;
                    }
                    break;
                }
                case acBetRaiseThree: {
                    if (expectedAction == acBetRaiseThreeAllIn) {
                        actionOkay = true;
                    }
                    break;
                }
                case acBetRaiseFour: {
                    if (expectedAction == acBetRaiseFourAllIn) {
                        actionOkay = true;
                    }
                    break;
                }
                default: {
                    break;
                } // end default
                } // end switch
                if (!actionOkay) {
                    Warning << "Player '" << summary_.name << "', id "
                        << summary_.id << " expected/actual action mismatch:\n"
                        << "    Expected: " << actionNames[expectedAction]
                        << "\n      Actual: " << actionNames[action]
                        << std::endl;
                }
            }
        }
    }
    summary_.pushedMoney += newlyPushed;
    summary_.stack -= newlyPushed;
    return newlyPushed;
}


ds::Money ds::Player::takeCall(Money totalBet) {
    if (summary_.allIn) {
        return 0;
    }

    std::pair<actionEnum, Money> actionPush = callFoldOption(totalBet);
    actionEnum action = actionPush.first;
    Money newlyPushed = actionPush.second;

    if (newlyPushed < 0) {
        // Player interacts based on actionEnums
        if (action == acCall) {
            newlyPushed = totalBet - summary_.pushedMoney;
            if (newlyPushed - summary_.stack < SMALL) {
                // Player is all in
                newlyPushed = summary_.stack;
                summary_.allIn = true;
            }
        } else if (action == acFold) {
            newlyPushed = 0;
            summary_.hasCards = false;
            pkt_ = PktCards(Card::unknownCard, Card::unknownCard);
        } else {
            Warning << "Player '" << summary_.name << "', id "
                << summary_.id << " chose '" << actionNames[action] << "', "
                << "which is not a valid action-based choice. Hand is folded."
                << std::endl;
            newlyPushed = 0;
            summary_.hasCards = false;
            pkt_ = PktCards(Card::unknownCard, Card::unknownCard);
        }
    } else {
        // Player interacts based on amount
        
        actionEnum expectedAction = acUnknown;
        if (newlyPushed < SMALL) {
            // Fold
            expectedAction = acFold;
            newlyPushed = 0;
            summary_.hasCards = false;
            pkt_ = PktCards(Card::unknownCard, Card::unknownCard);
        } else if (newlyPushed - summary_.stack < SMALL) {
            // Player is all in
            expectedAction = acCallAllIn;
            newlyPushed = summary_.stack;
            summary_.allIn = true;
        } else if (
            std::abs(summary_.pushedMoney + newlyPushed - totalBet) < SMALL
        ) {
            // Player calls
            expectedAction = acCall;
        }
        
        if (action != acUnknown) {
            // Player wants to double-check action
            if (action != expectedAction) {
                if (action != acCall || expectedAction != acCallAllIn) {
                    Warning << "Player '" << summary_.name << "', id "
                        << summary_.id << " expected/actual action mismatch:\n"
                        << "    Expected: " << actionNames[expectedAction]
                        << "\n      Actual: " << actionNames[action]
                        << std::endl;
                }
            }
        }
    }
    summary_.pushedMoney += newlyPushed;
    summary_.stack -= newlyPushed;
    return newlyPushed;
}


void ds::Player::askRevealWinningCards() {
    parseRevealOption(revealWinningCardsOption());
}


void revealWinningCardsOption() {
    // Default implementation
    return -1;
}


void ds::Player::askRevealLosingCards() {
    parseRevealOption(revealLosingCardsOption());
}


void revealLosingCardsOption() {
    // Default implementation
    return -1;
}


// ****** Protected Member Functions ****** //

const ds::PktCards& ds::Player::pocket() const {
    return pkt_;
}


// ****** Private Member Functions ****** //

void ds::Player::setWaitingForButton(bool newValue) {
    summary_.waitingForButton = newValue;
}


ds::Money ds::Player::collect(Money amount) {
    Money collectedAmount = amount;
    if (amount > summary_.stack) {
        summary_.allIn = true;
        collectedAmount = summary_.stack;
    }
    summary_.stack -= collectedAmount;
    summary_.pushedMoney += collectedAmount;
    return collectedAmount;
}


void ds::Player::dealPocket(VecDeckInd cardIndices) {
    pkt_ = PktCards(cardIndices);
}

void ds::Player::showPocket() {
    summary_.revealedPkt = pkt_;
}


void ds::Player::reward(Money amount) {
    summary_.rewardedMoney = amount;
}


void returnExcess(Money amount) {
    // Excess was already collected into a pot, it goes straight back into
    // the stack
    summary_.stack_ += amount;
}
            

void clearPushedMoney() {
    summary_.pushedMoney = 0;
}


void ds::Player::parseRevealOption(int answer) {
    switch (answer) {
    case -1: {
        // Do nothing
        break;
    }
    case 0: {
        summary_.revealedPkt.first = pkt_.first;
        break;
    }
    case 1: {
        summary_.revealedPkt.second = pkt_.second;
        break;
    }
    case 2: {
        summary_.revealedPkt = pkt_;
        break;
    }
    case default: {
        Warning << "Player '" << summary_.name << "', id " << summary_.id
            << " answered " << answer << " when acceptable range is -1..2. "
            << "Not revealing any cards." << std::endl;
        break;
    } // end default
    } // end switch
}

// ****** END ****** //
