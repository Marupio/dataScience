
#include <algorithm>
#include <Board.h>


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


ds::Board::Board(VecToken& vt) {
    reserveSpace();
    std::stringstream ss;
    ss << vt;
    ss >> *this;
}


ds::Board::Board(DeckInd dA, DeckInd dB, DeckInd dC) {
    reserveSpace();
    flop(dA, dB, dC);
}


ds::Board::Board(DeckInd dA, DeckInd dB, DeckInd dC, DeckInd dD) {
    reserveSpace();
    flop(dA, dB, dC);
    turn(dD);
}


ds::Board::Board(DeckInd dA, DeckInd dB, DeckInd dC, DeckInd dD, DeckInd dE) {
    reserveSpace();
    flop(dA, dB, dC);
    turn(dD);
    river(dD);
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


void ds::Board::flop(DeckInd diA, DeckInd diB, DeckInd diC) {
    if (cards_.size()) {
        FatalError << "Flop delivered to non-empty board.  Board cards are:\n"
            << cards_ << "\nAnd delivered cards are:\n"
            << diA << " " << diB << " " << diC << std::endl;
        abort();
    }
    cards_.push_back(Card(diA));
    cards_.push_back(Card(diB));
    cards_.push_back(Card(diC));
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


void ds::Board::clear() {
    cards_.clear();
    updateDerivedData();
}


ds::CardVal ds::Board::lowestValue() const {
    if (!stats_.size()) {
        return Card::lowAce;
    }
    return stats_.back().value();
}


ds::CardVal ds::Board::lowestValue(CardVal avoid) const {
    auto itR = stats_.crbegin();
    CardVal lowVal = Card::lowAce;
    for (
        ;
        itR != stats_.crend()
     && itR->value() == avoid;
        ++itR
    ) {}
    if (itR != stats_.crend()) {
        lowVal = itR->value();
    }
    return lowVal;
}


ds::CardVal ds::Board::lowestValue(const PktVals& avoid) const {
    auto itR = stats_.crbegin();
    CardVal lowVal = Card::lowAce;
    for (
        ;
        itR != stats_.crend()
     && (itR->value() == avoid.first || itR->value() == avoid.second);
        ++itR
    ) {}
    if (itR != stats_.crend()) {
        lowVal = itR->value();
    }
    return lowVal;
}


ds::PktVals ds::Board::lowestTwoValues() const {
    PktVals lowVals(PktVals::lowAces);
    auto itR = stats_.crbegin();
    if (itR != stats_.crend()) {
        lowVals.first = itR->value();
        ++itR;
        if (itR != stats_.crend()) {
            lowVals.second = itR->value();
        }
    }
    return lowVals;
}


ds::PktVals ds::Board::lowestTwoValues(CardVal avoid) const {
    PktVals lowVals(PktVals::lowAces);
    auto itR = stats_.crbegin();
    for (; itR != stats_.crend() && itR->value() == avoid; ++itR) {}
    if (itR != stats_.crend()) {
        lowVals.first = (itR++)->value();
        for (; itR != stats_.crend() && itR->value() == avoid; ++itR) {}
        if (itR != stats_.crend()) {
            lowVals.second = itR->value();
        }
    }
    return lowVals;
}


ds::PktVals ds::Board::lowestTwoValues(const PktVals& avoid) const {
    PktVals lowVals(PktVals::lowAces);
    auto itR = stats_.crbegin();
    for (
        ;
        itR != stats_.crend()
     && (itR->value() == avoid.first || itR->value() == avoid.second);
        ++itR
    ) {}
    if (itR != stats_.crend()) {
        lowVals.first = (itR++)->value();
        for (
            ;
            itR != stats_.crend()
         && (itR->value() == avoid.first || itR->value() == avoid.second);
            ++itR
        ) {}
        if (itR != stats_.crend()) {    
            lowVals.second = itR->value();
        }
    }
    return lowVals;
}


ds::CardVal ds::Board::highestValue() const {
    if (!stats_.size()) {
        return Card::lowAce;
    }
    return stats_.front().value();
}


ds::CardVal ds::Board::highestValue(CardVal avoid) const {
    CardVal highVal = Card::lowAce;
    auto it = stats_.cbegin();
    for (; it != stats_.cend() && it->value() == avoid; ++it) {}
    if (it != stats_.cend()) {
        highVal = it->value();
    }
    return highVal;
}


ds::CardVal ds::Board::highestValue(const PktVals& avoid) const {
    CardVal highVal = Card::lowAce;
    auto it = stats_.cbegin();
    for (
        ;
        it != stats_.cend()
     && (it->value() == avoid.first || it->value() == avoid.second);
        ++it
    ) {}
    if (it != stats_.cend()) {
        highVal = it->value();
    }
    return highVal;
}


ds::PktVals ds::Board::highestTwoValues() const {
    PktVals highVals(PktVals::lowAces);
    auto it = stats_.cbegin();
    if (it != stats_.cend()) {
        highVals.first = it->value();
        ++it;
        if (it != stats_.cend()) {
            highVals.second = it->value();
        }
    }
    return highVals;
}


ds::PktVals ds::Board::highestTwoValues(CardVal avoid) const {
    PktVals highVals(PktVals::lowAces);
    auto it = stats_.cbegin();
    for (; it != stats_.cend() && it->value() == avoid; ++it) {}
    if (it != stats_.cend()) {
        highVals.first = (it++)->value();
        for (; it != stats_.cend() && it->value() == avoid; ++it) {}
        if (it != stats_.cend()) {
            highVals.second = it->value();
        }
    }
    return highVals;
}


ds::PktVals ds::Board::highestTwoValues(const PktVals& avoid) const {
    PktVals highVals(PktVals::lowAces);
    auto it = stats_.cbegin();
    for (
        ;
        it != stats_.cend()
     && (it->value() == avoid.first || it->value() == avoid.second);
        ++it
    ) {}
    if (it != stats_.cend()) {
        highVals.first = (it++)->value();
        for (
            ;
            it != stats_.cend()
         && (it->value() == avoid.first || it->value() == avoid.second);
            ++it
        ) {}
        if (it != stats_.cend()) {
            highVals.second = it->value();
        }
    }
    return highVals;
}


ds::CardVal ds::Board::lowestFlushValue() const {
    if (flushValues_.size() < 4) {
        return Card::lowAce;
    }
    return flushValues_.back();
}


ds::PktVals ds::Board::lowestTwoFlushValues() const {
    switch(flushValues_.size()) {
    case 0: // fall through
    case 3: {
        return PktVals::lowAces;
    }
    case 4: {
        return PktVals(flushValues_.back(), Card::lowAce);
    }
    case 5: {
        auto itR = flushValues_.crbegin();
        PktVals lowVals;
        lowVals.first = *(itR++);
        lowVals.second = *itR;
        return lowVals;
    }
    default: {
        FatalError << "Unexpected size of flushValues (" << flushValues_.size()
            << ")" << std::endl;
        abort();
        return PktVals::lowAces;
    } // end default
    } // end switch
}


ds::CardVal ds::Board::highestFlushValue() const {
    if (!flushValues_.size()) {
        return Card::lowAce;
    }
    return flushValues_.front();
}


int ds::Board::perfectFlopHash() const {
    if (cards_.size() != flopSize_) {
        FatalError << "Requires board to be at flop, board contains: " << cards_ << std::endl;
        abort();
    }

    VecDeckInd vdi;
    vdi.reserve(3);
    for (auto it = cbegin(); it != cend(); ++it) {
        vdi.push_back(it->deckIndex());
    }
    std::sort(vdi.rbegin(), vdi.rend());
    int hash = 0;
    auto it = vdi.cbegin();
    for (int offset = 2; offset > 0; --offset) {
        hash += *it - offset;
        hash *= 50;
        ++it;
    }
    hash += *it;
    return hash;
}


ds::Board ds::Board::makeBoardFromFlopHash(int hash) {
    int c = hash%50;
    hash -= c;
    hash /= 50;
    int b = hash%50 + 1;
    hash -= b - 1;
    hash /= 50;
    int a = hash + 2;
    VecDeckInd vdi;
    vdi.reserve(3);
    vdi.push_back(a);
    vdi.push_back(b);
    vdi.push_back(c);
    return Board(vdi);
}


int ds::Board::perfectTurnHash() const {
    if (cards_.size() != sizeBeforeRiver_) {
        FatalError << "Requires board to be at turn, board contains: " << cards_ << std::endl;
        abort();
    }

    VecDeckInd vdi;
    vdi.reserve(4);
    for (auto it = cbegin(); it != cend(); ++it) {
        vdi.push_back(it->deckIndex());
    }
    std::sort(vdi.rbegin(), vdi.rend());
    int hash = 0;
    auto it = vdi.cbegin();
    for (int offset = 3; offset > 0; --offset) {
        hash += *it - offset;
        hash *= 49;
        ++it;
    }
    hash += *it;
    return hash;
}


ds::Board ds::Board::makeBoardFromTurnHash(int hash) {
    int d = hash%49;
    hash -= d;
    hash /= 49;
    int c = hash%49 + 1;
    hash -= c - 1;
    hash /= 49;
    int b = hash%49 + 2;
    hash -= b - 2;
    hash /= 49;
    int a = hash + 3;
    VecDeckInd vdi;
    vdi.reserve(4);
    vdi.push_back(a);
    vdi.push_back(b);
    vdi.push_back(c);
    vdi.push_back(d);
    return Board(vdi);
}


int ds::Board::perfectRiverHash() const {
    if (cards_.size() != maxCardsOnBoard_) {
        FatalError << "Requires board to be at river, board contains: " << cards_ << std::endl;
        abort();
    }

    VecDeckInd vdi;
    vdi.reserve(5);
    for (auto it = cbegin(); it != cend(); ++it) {
        vdi.push_back(it->deckIndex());
    }
    std::sort(vdi.rbegin(), vdi.rend());
    int hash = 0;
    auto it = vdi.cbegin();
    for (int offset = 4; offset > 0; --offset) {
        hash += *it - offset;
        hash *= 48;
        ++it;
    }
    hash += *it;
    return hash;
}


ds::Board ds::Board::makeBoardFromRiverHash(int hash) {
    int e = hash%48;
    hash -= e;
    hash /= 48;
    int d = hash%48 + 1;
    hash -= d - 1;
    hash /= 48;
    int c = hash%48 + 2;
    hash -= c - 2;
    hash /= 48;
    int b = hash%48 + 3;
    hash -= b -3;
    hash /= 48;
    int a = hash + 4;
    VecDeckInd vdi;
    vdi.reserve(5);
    vdi.push_back(a);
    vdi.push_back(b);
    vdi.push_back(c);
    vdi.push_back(d);
    vdi.push_back(e);
    return Board(vdi);
}


// ****** Private member functions ****** //

void ds::Board::reserveSpace() {
    cards_.reserve(maxCardsOnBoard_);
    flushValues_.reserve(maxCardsOnBoard_);
}


void ds::Board::updateDerivedData() {
    stats_ = VecValStats(cards_);
    suitCounts_.fill(0);
    flushSuit_ = Card::unknownSuit;
    flushValues_.clear();
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
                    flushValues_.push_back(itS->value());
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
                std::cerr << "\n" << cards_ << std::endl;
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
                    FatalError << "Expecting two missing suits from: ";
                    writeSuitCount(itS->suits(), std::cerr);
                    std::cerr << "\n" << cards_ << std::endl;
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
                    FatalError << "Expecting two missing suits from: ";
                    writeSuitCount(itS->suits(), std::cerr);
                    std::cerr << "\n" << cards_ << std::endl;
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

// ****** END ****** //