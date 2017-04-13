#include<algorithm>
#include<Board.h>


// ****** Static data ****** //

const short ds::Board::maxCardsOnBoard_(5);
const short ds::Board::flopSize_(3);
const short ds::Board::sizeBeforeRiver_(4);


// ****** Constructors ****** //

ds::Board::Board()
{
    reserveSpace();
    updateDerivedData();
}

ds::Board::Board(VecDeckInd vd) {
    reserveSpace();
    #ifdef DSDEBUG
    if (vd.size() < flopSize_ || vd.size() > maxCardsOnBoard_) {
        FatalError << "Unexpected number of initial board cards. Deck "
            << "indices are:\n" << vd << std::endl;
        abort();
    }
    #endif
    for (VecDeckInd::const_iterator it = vd.begin(); it != vd.end(); ++it) {
        cards_.push_back(Card(*it));
    }
    updateDerivedData();
}


ds::Board::Board(std::istream& is) {
    reserveSpace();
    is >> *this;
}


// ****** Public member functions ****** //

void ds::Board::flop(const VecDeckInd& vd) {
    if (cards_.size()) {
        FatalError << "Flop delivered to non-empty board.  Board cards are:\n"
            << cards_ << "\nAnd delivered cards are:\n"
            << vd << std::endl;
        abort();
    }
    if (vd.size() != flopSize_) {
        FatalError << "Unexpected flop size. Delivered cards are:\n"
            << vd << std::endl;
        abort();
    }
    for (VecDeckInd::const_iterator it = vd.begin(); it != vd.end(); ++it) {
        cards_.push_back(Card(*it));
    }
    updateDerivedData();
}


void ds::Board::turn(DeckInd di) {
    if (cards_.size() != flopSize_) {
        FatalError << "Turn delivered out-of-sequence.  Board cards are:\n"
            << cards_ << "\nAnd delivered card is: " << di << std::endl;
        abort();
    }
    cards_.push_back(Card(di));
    updateDerivedData();
}


void ds::Board::river(DeckInd di) {
    if (cards_.size() != sizeBeforeRiver_) {
        FatalError << "River delivered out-of-sequence.  Board cards are:\n"
            << cards_ << "\nAnd delivered card is: " << di << std::endl;
        abort();
    }
    cards_.push_back(Card(di));
    updateDerivedData();
}


ds::CardVal ds::Board::lowestVal() const {
    if (!stats_.size()) {
        return Card::lowAce;
    }
    return stats_.back().value();
}


ds::CardVal ds::Board::lowestVal(CardVal avoid) {
    auto itR = stats_.crbegin();
    CardVal lowVal = Card::lowAce;
    while (
        itR != stats_.crend()
     && (lowVal == Card::lowAce || lowVal == avoid)
    ) {
        lowVal = itR->value();
        ++itR;
    }
    return lowVal;
}


ds::CardVal ds::Board::lowestVal(const PktVals& avoid) {
    auto itR = stats_.crbegin();
    CardVal lowVal = Card::lowAce;
    while (
        itR != stats_.crend()
     && (
            lowVal == Card::lowAce
         || lowVal == avoid.first
         || lowVal == avoid.second
        )
    ) {
        lowVal = itR->value();
        ++itR;
    }
    return lowVal;
}


ds::PktVals ds::Board::lowestTwoVals() const {
    PktVals lowVals(PktVals::lowAces);
    auto itR = stats_.crbegin();
    while (itR != stats_.crend() && lowVals.first == Card::lowAce) {
        lowVals.first = Card::lowAce;
        ++itR;
    }
    while (itR != stats_.crend() && lowVals.second == Card::lowAce) {
        lowVals.second = Card::lowAce;
        ++itR;
    }
    return lowVals;
}


ds::PktVals ds::Board::lowestTwoVals(CardVal avoid) const {
    PktVals lowVals(PktVals::lowAces);
    auto itR = stats_.crbegin();
    while (
        itR != stats_.crend()
     && (lowVals.first == Card::lowAce || lowVals.first == avoid)
    ) {
        lowVals.first = Card::lowAce;
        ++itR;
    }
    while (
        itR != stats_.crend()
     && (lowVals.second == Card::lowAce || lowVals.second == avoid)
    ) {
        lowVals.second = Card::lowAce;
        ++itR;
    }
    return lowVals;
}


ds::PktVals ds::Board::lowestTwoVals(const PktVals& avoid) const {
    PktVals lowVals(PktVals::lowAces);
    auto itR = stats_.crbegin();
    while (
        itR != stats_.crend()
     && (
            lowVals.first == Card::lowAce
         || lowVals.first == avoid.first
         || lowVals.first == avoid.second
        )
    ) {
        lowVals.first = Card::lowAce;
        ++itR;
    }
    while (
        itR != stats_.crend()
     && (
            lowVals.second == Card::lowAce
         || lowVals.second == avoid.first
         || lowVals.second == avoid.second
        )
    ) {
        lowVals.second = Card::lowAce;
        ++itR;
    }
    return lowVals;
}


ds::CardVal ds::Board::highestVal() const {
    if (!stats_.size()) {
        return Card::lowAce;
    }
    return stats_.front().value();
}


ds::CardVal ds::Board::highestVal(CardVal avoid) const {
    CardVal highVal = Card::lowAce;
    auto it = stats_.cbegin();
    while (
        it != stats_.cend()
     && (highVal == Card::lowAce || highVal == avoid)
    ) {
        highVal = it->value();
        ++it;
    }
    return highVal;
}


ds::CardVal ds::Board::highestVal(const PktVals& avoid) const {
    CardVal highVal = Card::lowAce;
    auto it = stats_.cbegin();
    while (
        it != stats_.cend()
     && (
            highVal == Card::lowAce
         || highVal == avoid.first
         || highVal == avoid.second
        )
    ) {
        highVal = it->value();
        ++it;
    }
    return highVal;
}


ds::PktVals ds::Board::highestTwoVals() const {
    PktVals highVals(PktVals::lowAces);
    auto it = stats_.cbegin();
    while (it != stats_.cend() && highVals.first == Card::lowAce) {
        highVals.first = Card::lowAce;
        ++it;
    }
    while (it != stats_.cend() && highVals.second == Card::lowAce) {
        highVals.second = Card::lowAce;
        ++it;
    }
    return highVals;
}


ds::PktVals ds::Board::highestTwoVals(CardVal avoid) const {
    PktVals highVals(PktVals::lowAces);
    auto it = stats_.cbegin();
    while (
        it != stats_.cend()
     && (highVals.first == Card::lowAce || highVals.first == avoid)
    ) {
        highVals.first = Card::lowAce;
        ++it;
    }
    while (
        it != stats_.cend()
     && (highVals.second == Card::lowAce || highVals.second == avoid)
    ) {
        highVals.second = Card::lowAce;
        ++it;
    }
    return highVals;
}


ds::PktVals ds::Board::highestTwoVals(const PktVals& avoid) const {
    PktVals highVals(PktVals::lowAces);
    auto it = stats_.cbegin();
    while (
        it != stats_.cend()
     && (
            highVals.first == Card::lowAce
         || highVals.first == avoid.first
         || highVals.first == avoid.second
        )
    ) {
        highVals.first = Card::lowAce;
        ++it;
    }
    while (
        it != stats_.cend()
     && (
            highVals.second == Card::lowAce
         || highVals.second == avoid.first
         || highVals.second == avoid.second
        )
    ) {
        highVals.second = Card::lowAce;
        ++it;
    }
    return highVals;
}


ds::CardVal ds::Board::lowestFlushVal() const {
    if (flushVals_.size() < 4) {
        return Card::lowAce;
    }
    return flushVals_.back();
}


ds::PktVals ds::Board::lowestTwoFlushVals() const {
    switch(flushVals_.size()) {
    case 0: // fall through
    case 3: {
        return PktVals::lowAces;
        break;
    }
    case 4: {
        return PktVals(flushVals_.back(), Card::lowAce);
        break;
    }
    case 5: {
        auto itR = flushVals_.crbegin();
        return PktVals(*(itR++), (*itR));
        break;
    }
    default: {
        FatalError << "Unexpected size of flushVals (" << flushVals_.size()
            << ")" << std::endl;
        abort();
        return PktVals::lowAces;
        break;
    } // end default
    } // end switch
}


ds::CardVal ds::Board::highFlushVal() const {
    if (!flushVals_.size()) {
        return Card::lowAce;
    }
    return flushVals_.front();
}


// ****** Private member functions ****** //

void ds::Board::reserveSpace() {
    cards_.reserve(maxCardsOnBoard_);
    flushVals_.reserve(maxCardsOnBoard_);
}


void ds::Board::updateDerivedData() {
    stats_ = VecValStats(cards_);
    suitCounts_.fill(0);
    flushSuit_ = Card::unknownSuit;
    flushVals_.clear();
    foak_ = Card::unknownValue;
    set_ = Card::unknownValue;
    setMissingSuit_ = Card::unknownSuit;
    pairA_ = Card::unknownValue;
    pairAMissingSuits_ = {Card::unknownSuit, Card::unknownSuit};
    pairB_ = Card::unknownValue;
    pairBMissingSuits_ = {Card::unknownSuit, Card::unknownSuit};

    if (!cards_.size()) {
        return;
    }

    for (VecCard::const_iterator it=cards_.begin(); it != cards_.end(); ++it) {
        suitCounts_[it->suit()]++;
    }

    for (
        SuitCount::const_iterator it = suitCounts_.begin();
        it != suitCounts_.end();
        ++it
    ) {
        if (*it > 2) {
            flushSuit_ = it - suitCounts_.begin();
            for (
                VecValStats::const_iterator itS = stats_.cbegin();
                itS != stats_.cend();
                ++itS
            ) {
                if (itS->suits()[flushSuit_] > 0) {
                    flushVals_.push_back(itS->value());
                }
            }
            break;
        }
    }
    for (
        VecValStats::const_iterator itS = stats_.cbegin();
        itS != stats_.cend();
        ++itS
    ) {
        switch(itS->nCards()) {
        case 4: {
            foak_ = itS->value();
            break;
        }
        case 3: {
            set_ = itS->value();
            const VecSuit missingSuits = getMissingSuits(itS->suits());
            #ifdef DSDEBUG
            if (missingSuits.size() != 1) {
                FatalError << "Expecting only one missing suit from: ";
                writeSuitCount(itS->suits(), std::cerr);
                std::cerr << std::endl;
                abort();
            }
            #endif
            setMissingSuit_ = missingSuits.front();
            break;
        }
        case 2: {
            if (pairA_ == Card::unknownValue) {
                pairA_ = itS->value();
                const VecSuit missingSuits = getMissingSuits(itS->suits());
                #ifdef DSDEBUG
                if (missingSuits.size() != 2) {
                    FatalError << "Expecting two missing suit from: ";
                    writeSuitCount(itS->suits(), std::cerr);
                    std::cerr << std::endl;
                    abort();
                }
                #endif
                pairAMissingSuits_ =
                    {missingSuits.front(), missingSuits.back()};
            } else {
                pairB_ = itS->value();
                const VecSuit missingSuits = getMissingSuits(itS->suits());
                #ifdef DSDEBUG
                if (missingSuits.size() != 2) {
                    FatalError << "Expecting two missing suit from: ";
                    writeSuitCount(itS->suits(), std::cerr);
                    std::cerr << std::endl;
                    abort();
                }
                #endif
                pairBMissingSuits_ =
                    {missingSuits.front(), missingSuits.back()};
            }
        }
        default: {
            break;
        } // end break
        } // end switch
    }
}


// ****** Global operators ****** //

std::ostream& ds::operator<<(std::ostream& os, const Board& b) {
    os << b.cards_;
    return os;
}

std::istream& ds::operator>>(std::istream& is, Board& b) {
    if (b.cards_.size()) {
        FatalError << "Reading into non-empty board. Board cards are:\n"
            << b << std::endl;
        abort();
    }
    is >> b.cards_;
    b.reserveSpace();
    b.updateDerivedData();
    return is;
}
