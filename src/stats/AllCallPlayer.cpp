#include <AllCallPlayer.h>
#include <Table.h>
#include <HandRanker.h>

// ****** Constructors ****** //

ds::AllCallPlayer::AllCallPlayer():
    Player(),
    winningFlopRank_(324, std::vector<short>(0)),
    winningTurnRank_(324, std::vector<short>(0)),
    winningRiverRank_(324, std::vector<short>(0)),
    losingFlopRank_(324, std::vector<short>(0)),
    losingTurnRank_(324, std::vector<short>(0)),
    losingRiverRank_(324, std::vector<short>(0)),
    winningFlopPredict_(324, std::vector<std::vector<short>>(0)),
    winningTurnPredict_(324, std::vector<std::vector<short>>(0)),
    losingFlopPredict_(324, std::vector<std::vector<short>>(0)),
    losingTurnPredict_(324, std::vector<std::vector<short>>(0))
{}


ds::AllCallPlayer::AllCallPlayer(size_t id, const std::string& name):
    Player(id, name),
    winningFlopRank_(324, std::vector<short>(0)),
    winningTurnRank_(324, std::vector<short>(0)),
    winningRiverRank_(324, std::vector<short>(0)),
    losingFlopRank_(324, std::vector<short>(0)),
    losingTurnRank_(324, std::vector<short>(0)),
    losingRiverRank_(324, std::vector<short>(0)),
    winningFlopPredict_(324, std::vector<std::vector<short>>(0)),
    winningTurnPredict_(324, std::vector<std::vector<short>>(0)),
    losingFlopPredict_(324, std::vector<std::vector<short>>(0)),
    losingTurnPredict_(324, std::vector<std::vector<short>>(0))
{}


// ****** Public Member Functions ****** //

const std::vector<std::vector<short>>&
ds::AllCallPlayer::winningFlopRank() const {
    return winningFlopRank_;
}


const std::vector<std::vector<short>>& ds::AllCallPlayer::winningTurnRank() const {
    return winningTurnRank_;
}


const std::vector<std::vector<short>>& ds::AllCallPlayer::winningRiverRank() const {
    return winningRiverRank_;
}


const std::vector<std::vector<short>>& ds::AllCallPlayer::losingFlopRank() const {
    return losingFlopRank_;
}


const std::vector<std::vector<short>>& ds::AllCallPlayer::losingTurnRank() const {
    return losingTurnRank_;
}


const std::vector<std::vector<short>>& ds::AllCallPlayer::losingRiverRank() const {
    return losingRiverRank_;
}


const std::vector<std::vector<std::vector<short>>>&
ds::AllCallPlayer::winningFlopPredict() const {
    return winningFlopPredict_;
}


const std::vector<std::vector<std::vector<short>>>&
ds::AllCallPlayer::winningTurnPredict() const {
    return winningTurnPredict_;
}


const std::vector<std::vector<std::vector<short>>>&
ds::AllCallPlayer::losingFlopPredict() const {
    return losingFlopPredict_;
}


const std::vector<std::vector<std::vector<short>>>&
ds::AllCallPlayer::losingTurnPredict() const {
    return losingTurnPredict_;
}


void ds::AllCallPlayer::clear() {
    std::vector<std::vector<short>>(324, std::vector<short>(0)).swap(winningFlopRank_);
    std::vector<std::vector<short>>(324, std::vector<short>(0)).swap(winningTurnRank_);
    std::vector<std::vector<short>>(324, std::vector<short>(0)).swap(winningRiverRank_);
    std::vector<std::vector<short>>(324, std::vector<short>(0)).swap(losingFlopRank_);
    std::vector<std::vector<short>>(324, std::vector<short>(0)).swap(losingTurnRank_);
    std::vector<std::vector<short>>(324, std::vector<short>(0)).swap(losingRiverRank_);
    std::vector<std::vector<std::vector<short>>>(
        324, std::vector<std::vector<short>>(0)
    ).swap(winningFlopPredict_);
    std::vector<std::vector<std::vector<short>>>(
        324, std::vector<std::vector<short>>(0)
    ).swap(winningTurnPredict_);
    std::vector<std::vector<std::vector<short>>>(
        324, std::vector<std::vector<short>>(0)
    ).swap(losingFlopPredict_);
    std::vector<std::vector<std::vector<short>>>(
        324, std::vector<std::vector<short>>(0)
    ).swap(losingTurnPredict_);
}


void ds::AllCallPlayer::observeEvent(eventEnum event) {
    switch (event) {
    case Player::evFlop: {
        HandRanker hr(table().board(), copyPkt_);
        flopRank_ = hr.rank();
        hr.predict(flopPredict_);
        break;
    }
    case Player::evTurn: {
        HandRanker hr(table().board(), copyPkt_);
        turnRank_ = hr.rank();
        hr.predict(turnPredict_);
        break;
    }
    case Player::evRiver: {
        HandRanker hr(table().board(), copyPkt_);
        riverRank_ = hr.rank();
        break;
    }
    default: {
        break;
        // Do nothing
    } // end default
    } // end switch
}


void ds::AllCallPlayer::observeResults() {
    copyPkt_.sort();
    const short suitedOffset = copyPkt_.suited() ? 168 : 0;
    size_t hashIndex =
        (copyPkt_.second.value() - 2)*13
      + copyPkt_.first.value() - 2
      + suitedOffset;
    if (summary().rewardedMoney > SMALL) {
        // I won
        winningFlopRank_[hashIndex].push_back(flopRank_);
        winningTurnRank_[hashIndex].push_back(turnRank_);
        winningRiverRank_[hashIndex].push_back(riverRank_);
        winningFlopPredict_[hashIndex].push_back(std::move(flopPredict_));
        winningTurnPredict_[hashIndex].push_back(std::move(turnPredict_));
        flopPredict_.clear();
        turnPredict_.clear();
    } else {
        // I lost
        losingFlopRank_[hashIndex].push_back(flopRank_);
        losingTurnRank_[hashIndex].push_back(turnRank_);
        losingRiverRank_[hashIndex].push_back(riverRank_);
        losingFlopPredict_[hashIndex].push_back(std::move(flopPredict_));
        losingTurnPredict_[hashIndex].push_back(std::move(turnPredict_));
        flopPredict_.clear();
        turnPredict_.clear();
    }
}


// ****** END ****** //
