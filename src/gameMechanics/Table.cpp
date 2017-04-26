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
    setStatus(sePreFlop);
    if (seated_[dealer_].second == nullptr) {
        moveDealerButton();
    }
    if (blinds_->ante > 0) {
        for (auto it = seated_.begin(); it!= seated_.end(); ++it) {
            if (it->second == nullptr) {
                continue;
            }
            it->second->ante(blinds_->ante());
        }
    }
    auto itp = dealer_;
    if (nPlayers_ != 2) {
        nextPlayer(itp);
    }
    itp->smallBlind(blinds_->smallBlind);
    auto itSb = itp;
    nextPlayer(itp);
    itp->bigBlind(blinds_->bigBlind);
    dealCards();
    nextPlayer(itp);
    if (!takeBets(itp)) {
        return;
    }
    board_.flop(deck_.draw(3));
    if (!takeBets(itSb)) {
        return;
    }
    board_.turn(deck_.draw());
    if (!takeBets(itSb)) {
        return;
    }
    board_.river(deck_.draw());
    takeBets(itSb);
}


void ds::Table::pause();
void ds::Table::disband();
void ds::Table::addPlayer(PlayerRef& player);
void ds::Table::addPlayers(VecPlayerRef& players);
ds::VecPlayerRef& ds::Table::leavingTable();
void ds::Table::setBlinds(const Blinds& newBlinds);


// ****** Private Member Functions ****** //

void ds::Table::takeBets(VecPlayerRef::iterator& it) {
    Money payToPlay = blinds_->bigBlind;
    
}

void ds::Table::dealCards() {
    deck_.shuffle();
    for (auto it = seated_.begin(); it!= seated_.end(); ++it) {
        if (it->second == nullptr) {
            continue;
        }
        it->second->dealPocket(deck_.draw(2));
    }
}

void ds::Table::nextPlayer(VecPlayerRef::iterator& it) {
    do {
        ++it;
        if (it == seated_.end()) {
            it = seated_.begin();
        }
    } while (it->second == nullptr);
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
    } while (dealer->second == nullptr);
}

// ****** END ****** //
