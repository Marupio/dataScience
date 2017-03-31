#include<algorithm>
#include<Board.h>


// ****** Static data ****** //

const short ds::Board::maxCardsOnBoard_(5);
const short ds::Board::flopSize_(3);
const short ds::Board::sizeBeforeRiver_(4);


// ****** Constructors ****** //

ds::Board::Board() {
    reserveSpace();
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

// ****** Private member functions ****** //

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


void ds::Board::reserveSpace() {
    cards_.reserve(maxCardsOnBoard_);
    sortedUniqueVals_.reserve(maxCardsOnBoard_);
    sortedUniqueValCounts_.reserve(maxCardsOnBoard_);
    sortedUniqueValSuits_.reserve(maxCardsOnBoard_);
    suitCounts_.resize(Card::nSuits, 0);
    sortedFlushVals_.reserve(maxCardsOnBoard_);
}


void ds::Board::updateDerivedData() {
    sortedUniqueVals_.clear();
    sortedUniqueValCounts_.clear();
    sortedUniqueValSuits_.clean();
    suitCounts_.assign(Card::nSuits, 0);
    flushSuit_ = Card::unknownSuit;
    sortedFlushVals_.clear();

    if (!cards_.size()) {
        return;
    }

    VecCard allCards;
    allCards.reserve(cards_.size());
    for (VecCard::const_iterator it=cards_.begin(); it != cards_.end(); ++it) {
        allCards.push_back(*it);
        suitCounts_[it->suit()]++;
    }
    std::sort(allCards.begin(), allCards.end());
    if (allCards.size()) {
        sortedUniqueVals_.push_back(allCards.front().value());
        sortedUniqueValCounts_.push_back(1);
        sortedUniqueValSuits_.push_back(VecSuit(1, allCards.front().suit()));
    }
    for (
        std::pair<VecCard::const_iterator, VecCard::const_iterator> it(
            allCards.begin(), allCards.begin() + 1);
        it.second != allCards.end();
        ++it.first, ++it.second
    ) {
        if (it->first.value() != it->second.value()) {
            sortedUniqueVals_.push_back(it->second.value());
            sortedUniqueValCounts_.push_back(1);
            sortedUniqueValSuits_.push_back(VecSuit(1, it->second.suit()));
        } else {
            sortedUniqueValCounts_.back()++;
            sortedUniqueValSuits_.back().push_back(it->second.suit());
        }
    }
    for (
        std::vector<short>::const_iterator it = suitsCounts_.begin();
        it != suitCounts_.end();
        ++it
    ) {
        if (*it > 2) {
            flushSuit_ = it - suits.begin();
            for (
                VecCard::const_iterator it=cards_.begin();
                it != cards_.end();
                ++it
            ) {
                if (it->suit() == flushSuit_) {
                    sortedFlushVals_.pushBack(it->value());
                }
            }
            std::sort(sortedFlushVals_.begin(), sortedFlushVals_.end());
            break;
        }
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
    return is;
}
