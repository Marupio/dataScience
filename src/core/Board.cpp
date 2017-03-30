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
    suitCounts_.resize(Card::nSuits, 0);
}


void ds::Board::updateDerivedData() {
    sortedUniqueVals_.clear();
    sortedUniqueValCounts_.clear();
    suitCounts_.assign(Card::nSuits, 0);

    if (!cards.size()) {
        return;
    }

    VecCardVal allVals;
    allVals.reserve(cards.size());
    for (VecCard::const_iterator it=cards_.begin(); it != cards_.end(); ++it) {
        allVals.push_back(it->value());
        suitCounts_[it->suit()]++;
    }
    std::sort(allVals.begin(), allVals.end());
    if (allVals.size()) {
        sortedUniqueVals_.push_back(allVals.front());
        sortedUniqueValCounts_.push_back(1);
    }
    for (
        VecCardVal::const_iterator it = allVals.begin(),
            VecCardVal::const_iterator itNext = it + 1;
        itNext != allVals.end();
        ++it, ++itNext
    ) {
        if (*it != *itNext) {
            sortedUniqueVals_.push_back(*itNext);
            sortedUniqueValCounts_.push_back(1);
        } else {
            sortedUniqueValCounts_.back()++;
        }
    }
}


// ****** Global operators ****** //

std::ostream& ds::operator<<(std::ostream& os, const Board& c) {
    os << "(" << cards_.front();
    for (
        VecCard::const_iterator it = cards_.begin();
        it != cards_.end();
        ++it
    ) {
        os << " " << *it;
    }
    os << ")";
    return os;
}

std::istream& ds::operator>>(std::istream& is, Board& c) {
    if (cards_.size()) {
        FatalError << "Reading into non-empty board. Board cards are:\n"
            << cards_ << std::endl;
        abort();
    }
    char firstChar(is.get());
    if (firstChar != '(') {
        FatalError << "Expecting '(' from istream.  Got '" << firstChar
            << "', which is int " << short(firstChar) << "." << std::endl;
        abort();
    }
    while (1) {
        int nxt(is.peek());
        if (std::isspace(nxt)) {
            is.get();
            continue;
        }
        else if (char(nxt) == ')') {
            is.get();
            break;
        } else if (nxt == EOF) {
            FatalError << "Unexpected EOF while reading board. Successfully "
                << "read cards:\n" << cards_ << std::endl;
            abort();
        }
        cards_.push_back(Card(is));
    }
    return is;
}
