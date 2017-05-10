#include<AllCallPlayer.h>
#include<Table.h>
#include<HandRanker.h>

// ****** Constructors ****** //

ds::AllCallPlayer::AllCallPlayer():
    Player(),
    nWon_(324, 0),
    nLost_(324, 0),
    winningFlopRankSum_(324, 0),
    winningTurnRankSum_(324, 0),
    winningRankSum_(324, 0),
    losingFlopRankSum_(324, 0),
    losingTurnRankSum_(324, 0),
    losingRankSum_(324, 0)
{}


ds::AllCallPlayer::AllCallPlayer(size_t id, const std::string& name):
    Player(id, name),
    nWon_(324, 0),
    nLost_(324, 0),
    winningFlopRankSum_(324, 0),
    winningTurnRankSum_(324, 0),
    winningRankSum_(324, 0),
    losingFlopRankSum_(324, 0),
    losingTurnRankSum_(324, 0),
    losingRankSum_(324, 0)
{}


// ****** Public Member Functions ****** //

const std::vector<unsigned long long>& ds::AllCallPlayer::nWon() const {
    return nWon_;
}


const std::vector<unsigned long long>& ds::AllCallPlayer::nLost() const {
    return nLost_;
}


const std::vector<unsigned long long>&
ds::AllCallPlayer::winningFlopRankSum() const {
    return winningFlopRankSum_;
}


const std::vector<unsigned long long>&
ds::AllCallPlayer::winningTurnRankSum() const {
    return winningTurnRankSum_;
}


const std::vector<unsigned long long>&
ds::AllCallPlayer::winningRankSum() const {
    return winningRankSum_;
}


const std::vector<unsigned long long>&
ds::AllCallPlayer::losingFlopRankSum() const {
    return losingFlopRankSum_;
}


const std::vector<unsigned long long>&
ds::AllCallPlayer::losingTurnRankSum() const {
    return losingTurnRankSum_;
}


const std::vector<unsigned long long>&
ds::AllCallPlayer::losingRankSum() const {
    return losingRankSum_;
}


void ds::AllCallPlayer::observeEvent(eventEnum event) {
    switch (event) {
    case Player::evFlop: {
        HandRanker hr(table().board(), copyPkt_);
        flopRank_ = hr.rank();
        break;
    }
    case Player::evTurn: {
        HandRanker hr(table().board(), copyPkt_);
        turnRank_ = hr.rank();
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
        nWon_[hashIndex]++;
        winningFlopRankSum_[hashIndex] += flopRank_;
        winningTurnRankSum_[hashIndex] += turnRank_;
        winningRankSum_[hashIndex] += riverRank_;
    } else {
        // I lost
        nLost_[hashIndex]++;
        losingFlopRankSum_[hashIndex] += flopRank_;
        losingTurnRankSum_[hashIndex] += turnRank_;
        losingRankSum_[hashIndex] += riverRank_;
    }
}


// ****** END ****** //
