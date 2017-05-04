#include<Table.h>
#include<GameManager.h>

// ****** Constructors ****** //

ds::Table::Table(
    size_t nSeats,
    const Blinds& blinds,
    bool allowFastFolds,
    int dramaticPause
):
    Seats(nSeats),
    dealer_(seated_.begin()),
    blinds_(&blinds),
    nextBlinds_(nullptr),
    allowFastFolds_(allowFastFolds),
    dramaticPause_(dramaticPause),
    status_(seEmpty),
    ppAction_(ppContinue)
{
    pushedMoney_.reserve(nSeats);
}


// ****** Member Functions ****** //

const ds::Board& ds::Table::board() const {
    return bd_;
}


bool ds::Table::allowFastFolds() const {
    return allowFastFolds_;
}


const ds::Blinds& ds::Table::blinds() const {
    return *blinds_;
}


void ds::Table::disband() {
    statusEnum tableStatus = status_.load();
    if (tableStatus != sePause || tableStatus != seEmpty) {
        FatalError << "Cannot disband a table that is still in action. Action "
            "= "<< int(tableStatus) << "." << std::endl;
        abort();
    }
    if (ppAction_.load() != ppDisband) {
        FatalError << "Table is not ready to disband." << std::endl;
        abort();
    }
    everyoneGoHome();
}


void ds::Table::playContinuous() {
    ppAction_.store(ppContinue);
    play();
}


void ds::Table::playOnceThenPause() {
    ppAction_.store(ppPause);
    play();
}


void ds::Table::playOnceThenDisband() {
    ppAction_.store(ppDisband);
    play();
}


void ds::Table::play() {
    checkReadyForPlay();

    do {
        while (1) {
            allInShowDown_ = false;
            status_.store(sePreFlop);
            if (*dealer_ == nullptr) {
                moveDealerButton();
            }
            ante(blinds_->ante);
            SeatedPlayer player = dealer_;
            if (nPlayers_.load() != 2) {
                // Small blind is next to dealer unless heads-up, where it is dealer
                nextActivePlayer(player);
            }
            
            // Small blind
            collectBlinds(player, blinds_->smallBlind);
            
            // First-to-act (after the flop) is small blind (unless heads-up)
            SeatedPlayer ftaPlayer = player;
            firstToShow_ = ftaPlayer;
            nextActivePlayer(player);
            if (nPlayers_.load() == 2) {
                // If heads-up, first-to-act after the flop is the big-blind
                ftaPlayer = player;
            }
            
            // Big blind
            collectBlinds(player, blinds_->bigBlind);
            SeatedPlayer bbPlayer = player;

            dealCards();
            nextActivePlayer(player);

            // check for fast folds and new players
            checkForFastFolds(bbPlayer);
            seatWaitingPlayers(dealer_);

            if (!takeBets(player)) {
                break;
            }
            dramaticPause();

            board_.flop(deck_.draw(3));
            status_.store(seFlop);
            if (!takeBets(ftaPlayer)) {
                break;
            }
            dramaticPause();

            board_.turn(deck_.draw());
            status_.store(seTurn);
            if (!takeBets(ftaPlayer)) {
                break;
            }
            dramaticPause();

            board_.river(deck_.draw());
            status_.store(seRiver);
            if (!takeBets(ftaPlayer)) {
                break;
            }

            // Do hand compares
            compareHands();
        }
        resetTable();
        if (nextBlinds_.load() != nullptr) {
            blinds_ = nextBlinds_.load();
            nextBlinds_.store(nullptr);
            shareEvent(Player::evBlindsChanged);
        }
    } while (ppAction_.load() == ppContinue);

    // check post play action
    if (ppAction_.load() == ppDisband) {
        everyoneGoHome();
    }
    status_.store(sePaused);
}


ds::Table::statusEnum ds::Table::status() const {
    return status_.load();
}


ds::Table::postPlayEnum ds::Table::postPlayAction() const {
    return ppAction_.load();
}


void ds::Table::setPostPlayAction(postPlayEnum ppe) {
    ppAction_.store(ppe);
}


void ds::Table::setTableToPause() {
    ppAction_.store(ppPause);
}


void ds::Table::setTableToContinuousPlay() {
    ppAction_store(ppContinue);
}


void ds::Table::setTableToDisband() {
    ppAction_.store(ppDisband);
}


void ds::Table::setBlinds(const Blinds& newBlinds) {
    nextBlinds_.store(&newBlinds);
}


// ****** Private Member Functions ****** //

void ds::Table::checkReadyForPlay() const {
    if (blinds_ == nullptr) {
        FatalError << "Attempting to play without blinds" << std::endl;
        abort();
    }
    if (nPlayers_.load() < 2) {
        FatalError << "Not enough players to start play" << std::endl;
        abort();
    }
}


void ds::Table::moveDealerButton() {
    nextPlayer(dealer_);
    if ((*dealer_)->waitingForButton()) {
        (*dealer_)->setWaitingForButton(false);
    }
}


void ds::Table::ante(Money amount) {
    pushedMoney_.clear();

    SeatedPlayer it = firstActivePlayer(dealer_);
    do {
        nextActivePlayer(it);
        pushedMoney_.push_back(PushedMoney((*it)->collect(amount), it));
    } while (it != dealer_);
    pots_.collect(pushedMoney_);
}


void ds::Table::collectBlinds(PlayerRef& player, Money blinds) {
    Money paid = (*player)->collect(blinds);
    pushedMoney_.push_back(PushedMoney(paid, player));
    if ((*player)->allIn()) {
        shareAction(player, Player::acBlindsAllIn, paid);
    } else {
        shareAction(player, Player::acBlinds, paid);
    }
}


void ds::Table::dealCards() {
    deck_.shuffle();
    SeatedPlayer it = firstActivePlayer(dealer_);
    do {
        nextActivePlayer(it);
        (*it)->dealPocket(deck_.draw(2));
    } while (it != dealer_);
}


void dramaticPause() {
    if (allInShowDown_ && dramaticPause_ > 0) {
        sleep(dramaticPause_ * 1000);
    }
}


void ds::Table::checkForFastFolds(const SeatedPlayer& sp, Money totalBet) {
    if (!allowFastFolds_) {
        return;
    }
    SeatedPlayer player(sp);
    nextCardedPlayer(player);
    while (player != sp) {
        if (!(*player)->allIn()) {
            if ((*player)->fastFoldOption(totalBet)) {
                ghostKick(player);
            }
        }
    }
}


bool ds::Table::takeBets(SeatedPlayer player) {
    if (allInShowDown_) {
        return true;
    }

    // When to stop the betting rounds - after every player has acted, but any
    // player who does a full raise resets the action - all other players get
    // another chance to act
    SeatedPlayer stopPlayer = player;
    
    // However, with an under-raise all in, the players who already acted can
    // only call or fold at this point.  These variables assist with that:
    //- Pointer to first player that can call only
    PlayerPtr callsOnlyPlayerPtr = nullptr;
    
    //- Upon reaching callsOnlyPlayerPtr, callsOnly is set to true
    bool callsOnly = false;
    
    // To keep track of allInShowDowns
    size_t nActiveNotAllIn = 0;

    Money totalBet = blinds_->bigBlind;
    Money minRaise = totalBet;
    nextCardedPlayer(player);
    do {
        if (*player == callOnlyPlayerPtr) {
            callsOnly = true;
        }
        auto itPushed = pushedMoney_.find(player);
        if (itPushed != pushedMoney_.end()) {
            Money alreadyPushed = itPushed->first;
            #ifdef DSDEBUG
            if (alreadyPushed != (*player)->pushedMoney()) {
                FatalError << "alreadyPushed (" << alreadyPushed << ") and "
                    << "player->pushedMoney (" << (*player)->pushedMoney()
                    << ") do not agree." << std::endl;
                abort();
            }
            #endif

            Money newlyPushed;
            if (callsOnly) {
                newlyPushed = (*player)->takeCall(totalBet);
            } else {
                newlyPushed = (*player)->takeBet(totalBet, minRaise);
            }
            if (!(*it)->hasCards()) {
                // Folded
                (*itPushed)->second = nullptr;
                shareAction(player, Player::acFold, 0);
                continue;
            }

            if (newlyPushed == 0) {
                // Tell everyone about it
                shareAction(player, Player::acCheck, totalBet);
                nActiveNotAllIn++;
            } else if (newlyPushed + alreadyPushed > totalBet) {
                (*itPushed)->first += newlyPushed;
                totalBet = (*itPushed)->first;
                Money raisedAmount = (*itPushed)->first - totalBet;

                raiseHelper(
                    raisedAmount,
                    totalBet,
                    minRaise,
                    stopPlayer,
                    player,
                    nActiveNotAllIn
                );
            } else if (newlyPushed + alreadyPushed <= totalBet) {
                // Tell everyone about it
                if ((*player)->allIn()) {
                    shareAction(player, Player::acCallAllIn, totalBet);
                } else {
                    shareAction(player, Player::acCall, totalBet);
                    nActiveNotAllIn++;
                }
            }
        } else {
            // Player has not pushed anything yet
            Money newlyPushed;
            if (callsOnly) {
                newlyPushed = (*player)->takeCall(totalBet);
            } else {
                newlyPushed = (*player)->takeBet(totalBet, minRaise);
            }

            if (!(*it)->hasCards()) {
                // Folded
                shareAction(player, Player::acFold, 0);
                continue;
            }

            if (newlyPushed > 0) {
                pushedMoney_.push_back(PushedMoney(newlyPushed, player));
                if (newlyPushed > totalBet) {
                    totalBet = newlyPushed;
                    Money raisedAmount = newlyPushed - totalBet;

                    raiseHelper(
                        raisedAmount,
                        totalBet,
                        minRaise,
                        stopPlayer,
                        player,
                        nActiveNotAllIn
                    );
                } else {
                    if ((*player)->allIn()) {
                        shareAction(player, Player::acCallAllIn, totalBet);
                    } else {
                        shareAction(player, Player::acCall, totalBet);
                        nActiveNotAllIn++;
                    }
                }
            }
        }
        nextCardedPlayer(player);
    } while (player != stopPlayer);
    collectPushedMoney();
    
    // Detect if only one player remaining in bet
    if (pots_.size() == 1 && pots_.front().second.size() == 1) {
        SeatedPlayer winner = pots_.front().second.front();
        (*winner)->reward(pots_.front().first);
        (*winner)->askRevealWinningCards();
        shareResults();
        return false;
    }
    
    // Detects an all-in showdown
    if (nActiveNotAllIn < 2) {
        activateAllInShowDown();
    }

    return true;
}


void ds::Table::compareHands() {
    // The types I'm dealing with
    //struct VecPot: public std::vector<Pot>
    //struct Pot: public std::pair<Money, VecSeatedPlayer>
    //typedef VecPlayerPtr::iterator SeatedPlayer;
    //typedef std::vector<SeatedPlayer> VecSeatedPlayer;

    // First, check if top pot contains one player, and remove it.
    if (pots_.size() > 1 && pots_.back().second.size() == 1) {
        Pot overPot(pots_.pop_back());
        SeatedPlayer player(*overPot.second.front());
        (*player)->returnExcess(overPot.first);
    }

    //for each pot, compare the players
    std::vector<VecSeatedPlayer> winnersInEachPot;
    for (auto potI = pots_.begin(); potI != pots_.end(); ++potI) {
        VecSeatedPlayer& players(potI->second);
        VecSeatedPlayer winners;
        #ifdef DSDEBUG
        if (!players.size()) {
            FatalError << "Pot contains zero players." << std::endl;
            abort();
        }
        #endif
        if (players.size() == 1) {
            winners.push_back(players.begin());
        } else {
            winners.push_back(players.begin());
            for (
                SeatedPlayer playerJ = players.begin() + 1;
                playerJ != players.end();
                ++playerJ
            ) {
                short res = HandRanker::compare(
                    bd_,
                    (*winners.front())->pocket(),
                    (*playerJ)->pocket()
                );
                switch (res) {
                case -1:
                    winners.clear();
                    winners.push_back(playerJ);
                    break;
                case 0:
                    winners.push_back(playerJ);
                    break;
                case 1:
                    // do nothing
                    break;
                }
            }
        }
        winnersInEachPot.push_back(winners);
    }

    if (pots_.size() == 1) {
        // winner(s) gets it and return
        VecSeatedPlayer& winners(winnersInEachPot.front());
        Money award = pots_.first/winners.size();
        for (auto itp = winners.begin(); itp != winners.end(); ++itp) {
            (*itp)->reward(award);
            (*itp)->askRevealWinningCards();
        }
        shareResults();
        return;
    }

    // List of all winners to assist with showdown, initialise with winners of
    // highest pot
    std::vector<size_t> allWinners;
    VecSeatedPlayer& winners(winnersInEachPot.back());
    for (auto itp = winners.begin(); itp != winners.end(); ++itp) {
        allWinners.push_back((*itp)->id());
    }

    // Compare the best player in each pot, in reverse order, and adjust pot
    // winners accordingly
    for (
        std::pair<
            std::vector<VecSeatedPlayer>::reverse_iterator,
            VecPot::reverse_iterator
        > itPair(winnersInEachPot.rbegin()+1, pots_.rbegin+1);
        itPair.second != pots_.rend();
        ++itPair.first, ++itPair.second
    ) {
        SeatedPlayer highPlayer = (itPair.first-1)->front();
        SeatedPlayer lowPlayer = itPair.first->front();
        short res = HandRanker::compare(
            bd_,
            (*highPlayer)->pocket(),
            (*lowPlayer)->pocket()
        );
        if (res > 1) {
            // High player wins, stomps lowPlayer
            itPair.first->clear();
            *itPair.first = *(itPair.first-1)
        } else if (res == 1) {
            // Split
            itPair.first->insert(
                itPair.first->end(),
                (itPair.first-1)->begin(),
                (itPair.first-1)->end()
            );
        } else {
            const VecSeatedPlayer& lowWinners(*itPair.first);
            for (
                auto itp = lowWinners.begin();
                itp != lowWinners.end();
                ++itp
            ) {
                allWinners.push_back((*itp)->id());
            }
        }
    }

    // Showdown
    SeatedPlayer showingPlayer = firstToShow_;
    do {
        if (
            find(
                allWinners.begin(),
                allWinners.end(),
                (*showingPlayer)->id()
            ) == allWinners.end()
        ) {
            // Not winner
            (*showingPlayer)->askRevealLosingCards();
        } else {
            // Winner
            (*showingPlayer)->showPocket();
        }
        nextCardedPlayer(showingPlayer);
    } while (showingPlayer != firstToShow_);
    shareResults();
}


void ds::Table::collectPushedMoney() {
    pots_.collect(pushedMoney_);
    SeatedPlayer player = firstActivePlayer(dealer_);
    do {
        (*player)->clearPushedMoney();
        nextActivePlayer(player);
    } while (player != dealer_);
}


void ds::Table::raiseHelper(
    Money raisedAmount,
    Money totalBet,
    Money& minRaise,
    SeatedPlayer& stopPlayer,
    PlayerPtr& callsOnlyPtr,
    const SeatedPlayer& player,
    size_t& nActiveNotAllIn
) {
    if (raisedAmount + SMALL < minRaise) {
        #ifdef DSDEBUG
        if (!(*player)->allIn()) {
            FatalError << "Under-raise detected without an associated all-in."
                << std::endl;
            abort();
        }

        // With an all-in under-raise:
        // * Players that already acted cannot reraise, they can only call.
        // * minRaise increases by one full bigBlind
        // * no wait, minRaise stays the same.
        // * this is an agressive action, so player is still firstToShow
        //minRaise += blinds_->bigBlind;
        firstToShow_ = player;
        callsOnlyPtr = *player;

        shareAction(player, Player::acBetRaiseUnderAllIn, totalBet);
        return;
    }
    int raiseFactor = (raisedAmount + SMALL)/minRaise;
    minRaise = std::max(minRaise, raisedAmount);
    stopPlayer = player;

    // Set player first to show for showdown
    firstToShow_ = player;

    // Tell everyone about it
    Player::actionEnum raiseAction = Player::acUnknown;
    if (raiseFactor == 1) {
        if ((*player)->allIn) {
            raiseAction = Player::acBetRaiseAllIn;
        } else {
            raiseAction = Player::acBetRaise;
            nActiveNotAllIn++;
        }
    } else if (raiseFactor == 2) {
        if ((*player)->allIn) {
            raiseAction = Player::acBetRaiseTwoAllIn;
        } else {
            raiseAction = Player::acBetRaiseTwo;
            nActiveNotAllIn++;
        }
    } else if (raiseFactor == 3) {
        if ((*player)->allIn) {
            raiseAction = Player::acBetRaiseThreeAllIn;
        } else {
            raiseAction = Player::acBetRaiseThree;
            nActiveNotAllIn++;
        }
    } else if (raiseFactor > 3) {
        if ((*player)->allIn) {
            raiseAction = Player::acBetRaiseFourAllIn;
        } else {
            raiseAction = Player::acBetRaiseFour;
            nActiveNotAllIn++;
        }
    } else {
        FatalError << "Unexpected raise ratio (" << raiseFactor
            << ") returned from player '" << (*player)->name()
            << "'." << std::endl;
        abort();
    }
    shareAction(player, raiseAction, totalBet);

    // check for fast folds and new players
    checkForFastFolds(player, totalBet);
    seatWaitingPlayers(dealer_);
}


void ds::Table::activateAllInShowDown() {
    SeatedPlayer player(dealer_);
    nextCardedPlayer(player);
    SeatedPlayer startedAt(player);
    do {
        (*player)->showPocket();
        nextCardedPlayer(player);
    } (while player != startedAt);
    allInShowDown_ = true;
}


void ds::Table::shareEvent(Player::eventEnum event) {
    SeatedPlayer tellPlayer = firstPlayer(dealer_);
    do {
        (*tellPlayer)->observeEvent(event);
        nextPlayer(tellPlayer);
    } while (tellPlayer != dealer_);
}


void ds::Table::shareAction(
    const SeatedPlayer& player,
    Player::actionEnum action,
    Money amount
) {
    SeatedPlayer tellPlayer(player);
    do {
        nextPlayer(tellPlayer);
        (*tellPlayer)->observeAction(player, action, amount);
    } while (tellPlayer != player);
}


void ds::Table::shareResults() {
    SeatedPlayer player = firstPlayer(dealer_);
    do {
        (*player)->observeResults();
        nextPlayer(player);
    } while (player != dealer_);
}


void ds::Table::resetTable() {
    clearGhostPlayers();
    resetPlayers();
    seatWaitingPlayers(dealer_);
    moveDealerButton();
}


void ds::Table::resetPlayers() {
    SeatedPlayer player = firstPlayer(dealer_);
    do {
        (*player)->reset();

        // Kick broke players
        if ((*player)->stack() < SMALL) {
            // Player is broke
            kick(player);
        }
        nextPlayer(player);
    } while (player != dealer);
}


void ds::Table::everyoneGoHome() {
    kickAllPlayers();
}


// ****** END ****** //
