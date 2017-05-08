#include<AllInPlayer.h>
#include<Table.h>
#include<HandRanker.h>

// ****** Constructors ****** //

ds::AllInPlayer::AllInPlayer(size_t id, const std::string& name):
    Player(table, id, name),
    nWon_(323, 0),
    nLost_(323, 0),
    winningFlopRankSum_(323, 0),
    winningTurnRankSum_(323, 0),
    winningRankSum_(323, 0),
    losingFlopRankSum_(323, 0),
    losingTurnRankSum_(323, 0),
    losingRankSum_(323, 0)
{}


// ****** Public Member Functions ****** //

const std::vector<unsigned long long>& ds::Player::nWon() const {
    return nWon_;
}


const std::vector<unsigned long long>& ds::Player::nLost() const {
    return nLost_;
}


const std::vector<unsigned long long>& ds::Player::winningFlopRankSum() const {
    return winningFlopRankSum_;
}


const std::vector<unsigned long long>& ds::Player::winningTurnRankSum() const {
    return winningTurnRankSum_;
}


const std::vector<unsigned long long>& ds::Player::winningRankSum() const {
    return winningRankSum_;
}


const std::vector<unsigned long long>& ds::Player::losingFlopRankSum() const {
    return losingFlopRankSum_;
}


const std::vector<unsigned long long>& ds::Player::losingTurnRankSum() const {
    return losingTurnRankSum_;
}


const std::vector<unsigned long long>& ds::Player::losingRankSum() const {
    return losingRankSum_;
}


void ds::AllInPlayer::observeEvent(eventEnum event) {
    switch (event) {
    case Player::evFlop: {
        HandRanker hr(table_.bd(), copyPkt_);
        flopRank_ = hr.rank();
        break;
    }
    case Player::evTurn: {
        HandRanker hr(table_.bd(), copyPkt_);
        turnRank_ = hr.rank();
        break;
    }
    case Player::evRiver: {
        HandRanker hr(table_.bd(), copyPkt_);
        riverRank_ = hr.rank();
        break;
    }
    default: {
        break;
        // Do nothing
    } // end default
    } // end switch
}

void ds::AllInPlayer::observeResults() {
    copyPkt_.sort();
    const short suitedOffset = copyPkt_.suited() ? 168 : 0;
    size_t hashIndex =
        (copyPkt_.second.value() - 2)*13
      + copyPkt_.first.value() - 2
      + suitedOffset;
    if (summary().rewardedMoney > SMALL) {
        // I won
        nWon_[hashIndex]++;
        winningHandFlopRankSum_[hashIndex] += flopRank;
        winningHandTurnRankSum_[hashIndex] += turnRank;
        winningHandRankSum_[hashIndex] += riverRank;
    } else {
        // I lost
        nLost_[hashIndex]++;
        losingHandFlopRankSum_[hashIndex] += flopRank;
        losingHandTurnRankSum_[hashIndex] += turnRank;
        losingHandRankSum_[hashIndex] += riverRank;
    }
}


// ****** END ****** //
