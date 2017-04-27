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
{}


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

    while (ppAction_.load() == ppContinue) {
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
            itp->smallBlind(blinds_->smallBlind);
            collectBlinds(itp, blinds_->smallBlind);
            
            // First-to-act (after the flop) is small blind (unless heads-up)
            auto itFta = itp;
            nextPlayer(itp);
            if (nPlayers_ == 2) {
                // If heads-up, first-to-act after the flop is the big-blind
                itFta = itp;
            }
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
        }

        moveDealerButton();
        // check post play action
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

void ds::Table::ante(Money an) {
    VecPushedMoney antedUp;
    antedUp.reserve(nPlayers());

    auto it = dealer_;
    do {
        nextActivePlayer(it);
        antedUp.push_back(PushedMoney(it->collect(an), it));
    } while (it != dealer_);
    collectPushedMoney(antedUp);
    pots_.collect(collectPushedMoney);
}


void ds::Table::takeBets(VecPlayerRef::iterator& it) {
    auto stopIt = it;
    Money totalBet = blinds_->bigBlind;
    Money minBet = costOfRound;
    nextBettingPlayer(it);
    do {
        Money nextBet = it->takeBet(totalBet, minBet);
        if (nextBet > 0) {
            totalBet = nextBet;
            stopIt = it;
        }
        nextBettingPlayer(it);
    } while (it != stopIt);
}

void ds::Table::dealCards() {
    deck_.shuffle();
    for (auto it = seated_.begin(); it!= seated_.end(); ++it) {
        if (it == nullptr) {
            continue;
        }
        it->dealPocket(deck_.draw(2));
    }
}


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
    do {
        ++dealer;
        if (dealer_ == seated_.end()) {
            dealer_ = seated_.begin();
        }
    } while (dealer_ == nullptr);
}

// ****** END ****** //
