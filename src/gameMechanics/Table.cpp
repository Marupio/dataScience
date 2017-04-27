#include<Table.h>
#include<GameManager.h>

// ****** Constructors ****** //

ds::Table::Table(
    const GameManager& gm,
    size_t nSeats,
    const Blinds& blinds
):
    gm_(gm),
    nSeats_(nSeats),
    seated_(nSeats, PlayerRef(-1, nullptr)),
    dealer_(seated_.begin()),
    nSeated_(0),
    leaving_(0),
    blinds_(&blinds),
    nextBlinds_(nullptr),
    status_(seEmpty)
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


size_t ds::Table::nPlayers() const {
    return nSeated_;
//    size_t nPlayers = 0;
//    for (auto it = seated_.cbegin(); it != seated_.cend(); ++it) {
//        if (it->second == nullptr) {
//            continue;
//        }
//        ++nPlayers;
//    }
//    return nPlayers;
}


size_t ds::Table::nLeaving() const {
    return leaving_.size();
}


const ds::Blinds& ds::Table::blinds() const {
    return *blinds_;
}


const ds::VecPlayerRef& ds::Table::players() const {
    return seated_;
}


void ds::Table::play() {
    checkReadyForPlay();
    status_.store(sePreFlop);
    if (seated_[dealer_] == nullptr) {
        moveDealerButton();
    }
    std::vector<Money> potLimits(0, 0);
    for (auto it = seated_.begin(); it!= seated_.end(); ++it) {
        if (it == nullptr) {
            continue;
        }
        Money antedUp = it->ante(blinds_->ante());
        if (antedUp < blinds_->ante()) {
            &&&
        }
    }
    auto itp = dealer_;
    if (nPlayers_ != 2) {
        // Small blind is next to dealer unless heads-up, where it is dealer
        nextPlayer(itp);
    }
    itp->smallBlind(blinds_->smallBlind);
    
    // First-to-act (after the flop) is small blind (unless heads-up)
    auto itFta = itp;
    nextPlayer(itp);
    if (nPlayers_ == 2) {
        // If heads-up, first-to-act after the flop is the big-blind
        itFta = itp;
    }
    itp->bigBlind(blinds_->bigBlind);
    dealCards();
    nextPlayer(itp);
    if (!takeBets(itp)) {
        return;
    }
    board_.flop(deck_.draw(3));
    status_.store(seFlop);
    if (!takeBets(itSb)) {
        return;
    }
    board_.turn(deck_.draw());
    status_.store(seTurn);
    if (!takeBets(itSb)) {
        return;
    }
    board_.river(deck_.draw());
    status_.store(seRiver);
    takeBets(itSb);
    moveDealerButton();
    // &&& add naseum
}


void ds::Table::pause();
void ds::Table::disband();
void ds::Table::addPlayer(PlayerRef& player);
void ds::Table::addPlayers(VecPlayerRef& players);
ds::VecPlayerRef& ds::Table::leavingTable();
void ds::Table::setBlinds(const Blinds& newBlinds);


// ****** Private Member Functions ****** //

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


void ds::Table::nextBettingPlayer(VecPlayerRef::iterator& it) {
    do {
        nextPlayer(it);
    } while (!(it->hasCards()));
}


void ds::Table::nextPlayer(VecPlayerRef::iterator& it) {
    do {
        ++it;
        if (it == seated_.end()) {
            it = seated_.begin();
        }
    } while (it == nullptr);
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
