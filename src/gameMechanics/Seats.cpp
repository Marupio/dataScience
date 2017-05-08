#include<Seats.h>

// ****** Constructors ****** //

ds::Seats::Seats(size_t nSeats):
    nSeats_(nSeats),
    nPlayers_(0),
    seated_(nSeats, PlayerPtr(nullptr)),
    waitingToLeave_(0),
    waitingToSit_(0)
{}


// ****** Member Functions ****** //

size_t ds::Seats::nSeats() const {
    return nSeats_.load();
}


size_t ds::Seats::nPlayers() const {
    return nPlayers_.load();
}


size_t ds::Seats::roomInQueue() const {
    std::lock_guard<std::mutex> guard(waitingToSitMutex_);
    return nSeats() - nPlayers() - waitingToSit_.size();
}


void ds::Seats::addPlayer(PlayerPtr& player) {
    if (waitingToSit_.size()) {
        FatalError << "Attempting to add players when waitingToSit exists."
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
            player->setTablePtr(static_cast<const Table>(*this));
            break;
        }
    }
    nPlayers_++;
}


void ds::Seats::addPlayers(VecPlayerPtr& players) {
    if (waitingToSit_.size()) {
        FatalError << "Attempting to add players when waitingToSit exists."
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
    std::lock_guard<std::mutex> guard(waitingToSitMutex_);
    waitingToSit_.push_back(player);
}


void ds::Seats::queuePlayers(VecPlayerPtr& players) {
    std::lock_guard<std::mutex> guard(waitingToSitMutex_);
    for (auto it = players.begin(); it != players.end(); ++it) {
        waitingToSit_.push_back(*it);
    }
}


void ds::Seats::takeLeavingPlayers(std::vector<size_t>& left) {
    std::lock_guard<std::mutex> guard(waitingToLeaveMutex_);
    waitingToLeave_.swap(left);
}


// ****** Protected Member Functions ****** //

const ds::VecPlayerPtr& ds::Seats::seated() const {
    return seated_;
}


ds::SeatedPlayer ds::Seats::firstPlayer(SeatedPlayer& st) {
    SeatedPlayer rtnSt(st);
    if (*st == nullptr) {
        nextPlayer(st);
    }
    return rtnSt;
}


ds::ConstSeatedPlayer ds::Seats::firstPlayer(ConstSeatedPlayer& st) const {
    ConstSeatedPlayer rtnSt(st);
    if (*st == nullptr) {
        nextPlayer(st);
    }
    return rtnSt;
}


void ds::Seats::nextPlayer(SeatedPlayer& st) {
    SeatedPlayer startedAt = st;
    do {
        ++st;
        if (st == seated_.end()) {
            st = seated_.begin();
        }
    } while (*st == nullptr && st != startedAt);

    if (st == startedAt) {
        FatalError << "No other player exists" << std::endl;
        abort();
    }
}


void ds::Seats::nextPlayer(ConstSeatedPlayer& cst) const {
    ConstSeatedPlayer startedAt = cst;
    do {
        ++cst;
        if (cst == seated_.end()) {
            cst = seated_.begin();
        }
    } while (*cst == nullptr && cst != startedAt);
    if (cst == startedAt) {
        FatalError << "No other player exists" << std::endl;
        abort();
    }
}


ds::SeatedPlayer ds::Seats::firstActivePlayer(SeatedPlayer& st) {
    SeatedPlayer rtnSt(st);
    if (*st == nullptr && !(*st)->waitingForButton()) {
        nextActivePlayer(st);
    }
    return rtnSt;
}


ds::ConstSeatedPlayer ds::Seats::firstActivePlayer(
    ConstSeatedPlayer& st
) const {
    ConstSeatedPlayer rtnSt(st);
    if (*st == nullptr && !(*st)->waitingForButton()) {
        nextActivePlayer(st);
    }
    return rtnSt;
}


void ds::Seats::nextActivePlayer(SeatedPlayer& st) {
    SeatedPlayer startedAt = st;
    do {
        nextPlayer(st);
    } while ((*st)->waitingForButton() && st != startedAt);
    if (st == startedAt) {
        FatalError << "No other active player exists" << std::endl;
        abort();
    }
}


void ds::Seats::nextActivePlayer(ConstSeatedPlayer& cst) const {
    ConstSeatedPlayer startedAt = cst;
    do {
        nextPlayer(cst);
    } while ((*cst)->waitingForButton() && cst != startedAt);
    if (cst == startedAt) {
        FatalError << "No other active player exists" << std::endl;
        abort();
    }
}


ds::SeatedPlayer ds::Seats::firstCardedPlayer(SeatedPlayer& st) {
    SeatedPlayer rtnSt(st);
    if (*st == nullptr && !(*st)->hasCards()) {
        nextCardedPlayer(st);
    }
    return rtnSt;
}


ds::ConstSeatedPlayer ds::Seats::firstCardedPlayer(
    ConstSeatedPlayer& st
) const {
    ConstSeatedPlayer rtnSt(st);
    if (*st == nullptr && !(*st)->hasCards()) {
        nextCardedPlayer(st);
    }
    return rtnSt;
}


void ds::Seats::nextCardedPlayer(SeatedPlayer& st) {
    SeatedPlayer startedAt = st;
    do {
        nextPlayer(st);
    } while (!(*st)->hasCards() && st != startedAt);
    if (st == startedAt) {
        FatalError << "No other carded player exists" << std::endl;
        abort();
    }
}


void ds::Seats::nextCardedPlayer(ConstSeatedPlayer& cst) const {
    ConstSeatedPlayer startedAt = cst;
    do {
        nextPlayer(cst);
    } while (!(*cst)->hasCards() && cst != startedAt);
    if (cst == startedAt) {
        FatalError << "No other carded player exists" << std::endl;
        abort();
    }
}


void ds::Seats::nextEmptySeat(SeatedPlayer& st) {
    SeatedPlayer startedAt = st;
    do {
        ++st;
        if (st == seated_.end()) {
            st = seated_.begin();
        }
    } while (*st != nullptr && st != startedAt);
    if (st == startedAt) {
        FatalError << "No other empty seat exists." << std::endl;
        abort();
    }
}


void ds::Seats::nextEmptySeat(ConstSeatedPlayer& cst) const {
    ConstSeatedPlayer startedAt = cst;
    do {
        ++cst;
        if (cst == seated_.end()) {
            cst = seated_.begin();
        }
    } while (*cst != nullptr && cst != startedAt);
    if (cst == startedAt) {
        FatalError << "No other empty seat exists." << std::endl;
        abort();
    }
}


void ds::Seats::addQueue() {
    std::lock_guard<std::mutex> guard(waitingToSitMutex_);
    addPlayers(waitingToSit_);
    waitingToSit_.clear();
}


void ds::Seats::kick(SeatedPlayer& sp) {
    std::lock_guard<std::mutex> guard(waitingToLeaveMutex_);
    if (*sp != nullptr) {
        (*sp)->observeEvent(Player::evLeavingTable);
        (*sp)->clearTablePtr();
        waitingToLeave_.push_back((*sp)->id());
        *sp = nullptr;
        nPlayers_--;
    }
}


void ds::Seats::kick(VecSeatedPlayer& vsp) {
    std::lock_guard<std::mutex> guard(waitingToLeaveMutex_);
    for (auto itVsp = vsp.begin(); itVsp != vsp.end(); ++itVsp) {
        if (**itVsp != nullptr) {
            waitingToLeave_.push_back((**itVsp)->id());
            (**itVsp)->observeEvent(Player::evLeavingTable);
            (**itVsp)->clearTablePtr();
            **itVsp = nullptr;
            nPlayers_--;
        }
    }
}


void ds::Seats::kickAllPlayers() {
    for (auto it = seated_.begin(); it != seated_.end(); ++it) {
        kick(it);
    }
}


void ds::Seats::ghostKick(SeatedPlayer& sp) {
    std::lock_guard<std::mutex> guard(waitingToLeaveMutex_);
    if (*sp != nullptr) {
        waitingToLeave_.push_back((*sp)->id());
        (*sp)->observeEvent(Player::evLeavingTable);
        (*sp)->clearTablePtr();
        ghostPlayers_.push_back(GhostPlayer((*sp)->summary()));
        *sp = &ghostPlayers_.back();
        (*sp)->setTablePtr(static_cast<const Table>(*this));
        ghostPlayerSeats_.push_back(sp);
    }
}


void ds::Seats::clearGhostPlayers() {
    for (
        auto it = ghostPlayerSeats_.begin();
        it != ghostPlayerSeats_.end();
        ++it
    ) {
        **it = nullptr;
    }
    ghostPlayers_.clear();
}


void ds::Seats::seatWaitingPlayers(SeatedPlayer dealer) {
    std::lock_guard<std::mutex> guard(waitingToSitMutex_);
    if (waitingToSit_.size()) {
        SeatedPlayer emptySeat = dealer;
        for (
            auto itSit = waitingToSit_.begin();
            itSit != waitingToSit_.end();
            ++itSit
        ) {
            nextEmptySeat(emptySeat);
            *emptySeat = &(**itSit);
            // Despite its name, emptySeat is not emptyAnymore
            (*sp)->setTablePtr(static_cast<const Table>(*this));
            (*emptySeat)->setWaitingForButton(true);
            (*emptySeat)->observeEvent(Player::evJoiningTable);
        }
    }
}


// ****** END ****** //
