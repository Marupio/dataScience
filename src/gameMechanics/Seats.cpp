#include<Seats.h>
#include<GameManager.h>

// ****** Constructors ****** //

ds::Seats::Seats(size_t nSeats):
    seated_(nSeats, PlayerPtr(nullptr)),
    leaving_(0),
    seatQueue_(0),
    nSeated_(0)
{}


// ****** Member Functions ****** //

size_t ds::Seats::nSeats() const {
    return seated_.size();
}


size_t ds::Seats::nPlayers() const {
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


size_t ds::Seats::nLeaving() const {
    return leaving_.size();
}


const ds::VecPlayerPtr& ds::Seats::seated() const {
    return seated_;
}


void ds::Seats::nextPlayer(SeatedPlayer& st) {
    do {
        ++st;
        if (st == seated_.end()) {
            st = seated_.begin();
        }
    } while (st == nullptr);
}


void ds::Seats::nextPlayer(ConstSeatedPlayer& cst) const {
    do {
        ++cst;
        if (cst == seated_.end()) {
            cst = seated_.begin();
        }
    } while (cst == nullptr);
}


void ds::Seats::nextActivePlayer(SeatedPlayer& st) {
    do {
        nextPlayer(st);
    } while (st->waitingForButton());
}


void ds::Seats::nextActivePlayer(ConstSeatedPlayer& cst) {
    do {
        nextPlayer(cst);
    } while (cst->waitingForButton());
}


void ds::Seats::nextCardedPlayer(SeatedPlayer& st) {
    do {
        nextPlayer(st);
    } while (!(st->hasCards()));
}


void ds::Seats::nextCardedPlayer(ConstSeatedPlayer& cst) const {
    do {
        nextPlayer(cst);
    } while (!(cst->hasCards()));
}


void ds::Seats::addPlayer(PlayerPtr& player) {
    if (seatQueue_.size()) {
        FatalError << "Attempting to add players when seatQueue exists."
            << std::endl;
        abort();
    }
    if (nPlayers() == nSeats()) {
        FatalError << "Attempting to add player '" << player->name() << "' ("
            << player->id() << ") to a full table." << std::endl;
        abort();
    }
    for (auto it = seated_.begin(); it != seated_.end(); ++it) {
        if (*it == nullptr) {
            *it = player;
            break;
        }
    }
    nPlayers_++;
}


void ds::Seats::addPlayers(VecPlayerPtr& players) {
    if (seatQueue_.size()) {
        FatalError << "Attempting to add players when seatQueue exists."
            << std::endl;
        abort();
    }
    if (nSeats() - nPlayers() < players.size()) {
        FatalError << "Attempting to add " << players.size() << " players "
            << "to a table with " << nSeats() << " seats occupied by "
            << nPlayers() << " players.  Capacity exceeded." << std::endl;
        abort();
    }
    auto itp = players.begin();
    for (auto it = seated_.begin(); it != seated_.end(); ++it) {
        if (*it == nullptr) {
            *it = *itp++;
            if (itp == players.end()) {
                break;
            }
        }
    }
    nPlayers_ += players.size();
}


void ds::Seats::queuePlayer(PlayerPtr& player) {
    seatQueue_.push_back(player);
}


void ds::Seats::queuePlayers(VecPlayerPtr& players) {
    for (auto it = players.begin(); it != players.end(); ++it) {
        seatQueue_.push_back(*it);
    }
}


void ds::Seats::addQueue() {
    addPlayers(seatQueue_);
    seatQueue_.clear();
}


void ds::Seats::kickPlayer(SeatedPlayer& sp) {
    if (*sp != nullptr) {
        leaving_.push_back(sp->id());
        *sp = nullptr;
    }
}


void ds::Seats::kickPlayers(VecSeatedPlayer& vsp) {
    for (auto itVsp = vsp.begin(); itVsp != vsp.end(); ++itVsp) {
        if (**itVsp != nullPtr) {
            leaving_.push_back((*itVsp)->id());
            *(*itVsp) = nullptr;
        }
    }
}


void ds::Seats::kickAllPlayers() {
    for (auto it = seated_.begin(); it != seated_.end(); ++it) {
        kickPlayer(it);
    }
}


ds::VecPlayerPtr& ds::Seats::leavingSeats() {
    return leaving_;
}


// ****** Private Member Functions ****** //

void ds::Seats::takeBets(VecPlayerPtr::iterator& it) {
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

void ds::Seats::dealCards() {
    deck_.shuffle();
    for (auto it = seated_.begin(); it!= seated_.end(); ++it) {
        if (it == nullptr) {
            continue;
        }
        it->dealPocket(deck_.draw(2));
    }
}


// ****** END ****** //
