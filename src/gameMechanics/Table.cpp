#include<Table.h>
#include<GameManager.h>

// ****** Constructors ****** //

ds::Table::Table(
    const GameManager& gm,
    size_t nSeats,
    const Blinds& blinds
):
    Seats(nSeats),
    gm_(gm),
    dealer_(seated_.begin()),
    blinds_(&blinds),
    nextBlinds_(nullptr),
    status_(seEmpty),
    ppAction_(ppContinue)
{
    pushedMoney_.reserve(nSeats);
}


// ****** Member Functions ****** //

const ds::Board& ds::Table::board() const {
    return bd_;
}


const ds::GameManager& ds::Table::manager() const {
    return gm_;
}


size_t ds::Table::nSeats() const {
    return nSeats_;
}


ds::Table::statusEnum ds::Table::status() const {
    return status_;
}


const ds::Blinds& ds::Table::blinds() const {
    return *blinds_;
}


void ds::Table::play() {
    checkReadyForPlay();

    do {
        while (1) {
            status_.store(sePreFlop);
            if (*dealer_ == nullptr) {
                moveDealerButton();
            }
            ante(blinds_->ante());
            SeatedPlayer player = dealer_;
            if (nPlayers_ != 2) {
                // Small blind is next to dealer unless heads-up, where it is dealer
                nextPlayer(player);
            }
            
            // Small blind
            collectBlinds(player, blinds_->smallBlind);
            
            // First-to-act (after the flop) is small blind (unless heads-up)
            SeatedPlayer ftaPlayer = player;
            firstToShow_ = ftaPlayer;
            nextPlayer(player);
            if (nPlayers_ == 2) {
                // If heads-up, first-to-act after the flop is the big-blind
                ftaPlayer = player;
            }
            
            // Big blind
            collectBlinds(player, blinds_->bigBlind);
            SeatedPlayer bbPlayer = player;

            dealCards();
            nextPlayer(player);

            // check for fast folds and new players
            checkForFastFolds(bbPlayer);
            checkForWaitingToSit();

            if (!takeBets(player)) {
                break;
            }
            board_.flop(deck_.draw(3));
            status_.store(seFlop);
            if (!takeBets(ftaPlayer)) {
                break;
            }
            board_.turn(deck_.draw());
            status_.store(seTurn);
            if (!takeBets(ftaPlayer)) {
                break;
            }
            board_.river(deck_.draw());
            status_.store(seRiver);
            if (!takeBets(ftaPlayer)) {
                break;
            }

            // Do hand compares
            compareHands();
        }
        moveDealerButton();
        clearGhostPlayers();
    } while (ppAction_.load() == ppContinue);

    // check post play action
    if (ppAction_.load() == ppDisband) {
        everyoneGoHome();
    }
}


void ds::Table::pause() {
    ppAction_.store(ppPause);
}


void ds::Table::resume() {
    ppAction_store(ppContinue);
}


void ds::Table::disband() {
    ppAction_.store(ppDisband);
}


void ds::Table::setBlinds(const Blinds& newBlinds);


// ****** Private Member Functions ****** //

void ds::Table::checkReadyForPlay() const {
    if (blinds_ == nullptr) {
        FatalError << "Attempting to play without blinds" << std::endl;
        abort();
    }
    if (nPlayers() < 2) {
        FatalError << "Not enough players to start play" << std::endl;
        abort();
    }
}


void ds::Table::moveDealerButton() {
    nextPlayer(dealer);
    if (dealer->waitingForButton()) {
        dealer->setWaitingForButton(false);
    }
}


void ds::Table::ante(Money an) {
    pushedMoney_.clear();

    auto it = dealer_;
    do {
        nextActivePlayer(it);
        pushedMoney_.push_back(PushedMoney(it->collect(an), it));
    } while (it != dealer_);
    pots_.collect(pushedMoney_);
}


void ds::Table::collectBlinds(PlayerRef& player, Money blinds) {
    pushedMoney_.push_back(PushedMoney(player->collect(blinds), player));
}


void ds::Table::dealCards() {
    deck_.shuffle();
    auto it = dealer_;
    do {
        nextActivePlayer(it);
        it->dealPocket(deck_.draw(2));
    } while (it != dealer_);
}


void ds::Table::checkForFastFolds(const SeatedPlayer& sp, Money totalBet) {
    if (!gm_.allowFastFolds()) {
        return;
    }
    SeatedPlayer player(sp);
    nextCardedPlayer(player);
    while (player != sp) {
        if (!player->allIn()) {
            if (player->fastFoldOption(totalBet)) {
                ghostKick(player);
            }
        }
    }
}


void ds::Table::checkForWaitingToSit() {
    if (seatQueue_.size()) {
        SeatedPlayer emptySeat = dealer_;
        
    }
}


void ds::Table::takeBets(SeatedPlayer player) {
    auto stopPlayer = player;
    
    Money totalBet = blinds_->bigBlind;
    Money minRaise = totalBet;
    nextCardedPlayer(player);
    do {
        auto itPushed = pushedMoney_.find(player);
        if (itPushed != pushedMoney_.end()) {
            Money alreadyPushed = itPushed->first;
            #ifdef DSDEBUG
            if (alreadyPushed != player->pushedMoney()) {
                FatalError << "alreadyPushed (" << alreadyPushed << ") and "
                    << "player->pushedMoney (" << player->pushedMoney()
                    << ") do not agree." << std::endl;
                abort();
            }
            #endif
            Money newlyPushed =
                player->takeBet(totalBet, minRaise);
            if (!it->hasCards()) {
                // Folded
                itPushed->second = nullptr;
                shareAction(player, Player::acFold);
                continue;
            }

            if (newlyPushed == 0) {
                // Tell everyone about it
                shareAction(player, Player::acCheck, totalBet);
            } else if (newlyPushed + alreadyPushed > totalBet) {
                itPushed->first += newlyPushed;
                minRaise = std::max(minRaise, itPushed->first - totalBet);
                totalBet = itPushed->first;
                stopPlayer = player;

                // Set player first to show for showdown
                firstToShow_ = player;

                // Tell everyone about it
                if (player->allIn()) {
                    shareAction(player, Player::acRaiseAllIn, totalBet);
                } else {
                    shareAction(player, Player::acRaise, totalBet);
                }

                // check for fast folds and new players
                checkForFastFolds(player, totalBet);
                checkForWaitingToSit();
            } else if (newlyPushed + alreadyPushed <= totalBet) {
                // Tell everyone about it
                if (player->allIn()) {
                    shareAction(player, Player::acCallAllIn, totalBet);
                } else {
                    shareAction(player, Player::acCall, totalBet);
                }
            }
        } else {
            // Player has not pushed anything yet
            Money newlyPushed = player->takeBet(totalBet, minRaise);

            if (!it->hasCards()) {
                // Folded
                shareAction(player, Player::acFold, 0);
                continue;
            }

            if (newlyPushed > 0) {
                pushedMoney_.push_back(PushedMoney(newlyPushed, player));
                if (newlyPushed > totalBet) {
                    minRaise = std::max(minRaise, newlyPushed - totalBet);
                    totalBet = newlyPushed;
                    stopPlayer = player;

                    // Set player first to show for showdown
                    firstToShow_ = player;

                    // Tell everyone about it
                    if (player->allIn()) {
                        shareAction(player, Player::acRaiseAllIn, totalBet);
                    } else {
                        shareAction(player, Player::acRaise, totalBet);
                    }

                    // check for fast folds and new players
                    checkForFastFolds(player, totalBet);
                    checkForWaitingToSit();
                } else {
                    if (player->allIn()) {
                        shareAction(player, Player::acCallAllIn, totalBet);
                    } else {
                        shareAction(player, Player::acCall, totalBet);
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
        winner->reward(pots_.front().first);
        winner->revealWinningCardsOption();
        shareResults();
        return false;
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
        player->returnExcess(overPot.first);
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
                    winners.front()->pocket(),
                    playerJ->pocket()
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
        Money award = pots_.first / winners.size();
        for (auto itp = winners.begin(); itp != winners.end(); ++itp) {
            itp->reward(award);
            itp->revealWinningCardsOption();
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
            highPlayer->pocket(),
            lowPlayer->pocket()
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
                showingPlayer->id()
            ) == allWinners.end()
        ) {
            // Not winner
            showingPlayer->revealLosingCardsOption();
        } else {
            // Winner
            showingPlayer->showPocket();
        }
        nextCardedPlayer(showingPlayer);
    } while (showingPlayer != firstToShow_);
    shareResults();
}


void ds::Table::collectPushedMoney() {
    pots_.collect(pushedMoney_);
    SeatedPlayer player = dealer_;
    do {
        player->clearPushedMoney();
        nextActivePlayer(player);
    } while (player != dealer_);
}


void ds::Table::shareAction(
    const SeatedPlayer& player,
    Player::actionEnum action,
    Money amount
) {
    SeatedPlayer tellPlayer(player);
    do {
        nextPlayer(tellPlayer);
        tellPlayer->observeAction(player, action, amount);
    } while (tellPlayer != player);
}


void ds::Table::shareResults() {
    SeatedPlayer player = dealer_;
    do {
        player->observeResults();
        nextPlayer(player);
    } while (player != dealer_);
}


void ds::Table::everyoneGoHome() {
    kickAllPlayers();
}


// ****** END ****** //
