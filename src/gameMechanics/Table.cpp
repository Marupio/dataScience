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
            auto itp = dealer_;
            if (nPlayers_ != 2) {
                // Small blind is next to dealer unless heads-up, where it is dealer
                nextPlayer(itp);
            }
            
            // Small blind
            collectBlinds(itp, blinds_->smallBlind);
            
            // First-to-act (after the flop) is small blind (unless heads-up)
            auto itFta = itp;
            nextPlayer(itp);
            if (nPlayers_ == 2) {
                // If heads-up, first-to-act after the flop is the big-blind
                itFta = itp;
            }
            
            // Big blind
            collectBlinds(itp, blinds_->bigBlind);

            dealCards();
            nextPlayer(itp);

            // check for fast folds and new players
            checkForFastFolds();
            checkForWaitingToSit();

            if (!takeBets(itp)) {
                break;
            }
            board_.flop(deck_.draw(3));
            status_.store(seFlop);
            if (!takeBets(itSb)) {
                break;
            }
            board_.turn(deck_.draw());
            status_.store(seTurn);
            if (!takeBets(itSb)) {
                break;
            }
            board_.river(deck_.draw());
            status_.store(seRiver);
            takeBets(itSb);

            // Do hand compares
            compareHands();

            moveDealerButton();
        } while (ppAction_.load() == ppContinue);

    // check post play action
    if (ppAction_.load() == ppDisband) {
        // Take some kind of action
        // &&&
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


void ds::Table::checkForFastFolds() {
// &&&
}


void ds::Table::checkForWaitingToSit() {
// &&&
}


void ds::Table::takeBets(VecPlayerRef::iterator& it) {
    auto stopIt = it;
    
    Money totalBet = blinds_->bigBlind;
    Money minRaise = totalBet;
    nextCardedPlayer(it);
    do {
        auto itPushed = pushedMoney_.find(it);
        if (itPushed != pushedMoney_.end()) {
            Money alreadyPushed = itPushed->first;
            Money newlyPushed = it->takeBet(totalBet, alreadyPushed, minRaise);
            if (!it->hasCards()) {
                // Folded
                itPushed->second = nullptr;
            }

            // check for fast folds and new players
            checkForFastFolds();
            checkForWaitingToSit();

            if (newlyPushed + alreadyPushed > totalBet) {
                itPushed->first += newlyPushed;
                minRaise = std::max(minRaise, itPushed->first - totalBet);
                totalBet = itPushed->first;
                stopIt = it;
            }
        } else {
            // Player has not pushed anything yet
            Money newlyPushed = it->takeBet(totalBet, 0, minRaise);

            // check for fast folds and new players
            checkForFastFolds();
            checkForWaitingToSit();

            if (newlyPushed > 0) {
                pushedMoney_.push_back(PushedMoney(newlyPushed, it));
                if (newlyPushed > totalBet) {
                    minRaise = std::max(minRaise, newlyPushed - totalBet);
                    totalBet = newlyPushed;
                    stopIt = it;
                }
            }
        }
        nextCardedPlayer(it);
    } while (it != stopIt);
    pots_.collect(pushedMoney_);
    
    // Detect if only one player remaining in bet
    if (pots_.size() == 1 && pots_.front().second.size() == 1) {
        pots_.front().second.front()->reward(pots_.front().first);
        return false;
    }
    return true;
}


void ds::Table::compareHands() {
    // &&&
}

// ****** END ****** //
