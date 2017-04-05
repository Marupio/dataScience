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
    values_.reserve(maxCardsOnBoard_);
    valueCounts_.reserve(maxCardsOnBoard_);
    valueSuits_.reserve(maxCardsOnBoard_);
    flushVals_.reserve(maxCardsOnBoard_);

    suitCounts_.fill(0);
    flushSuit_ = Card::unknownSuit;
    foak_ = Card::unknownValue;
    toak_ = Card::unknownValue;
    pairs_ = PktVals(Card::unknownValue, Card::unknownValue);
}


void ds::Board::updateDerivedData() {
    stats_.clear();
    stats_ = VecValStats(cards_);
    suitCounts_.fill(0);
    flushSuit_ = Card::unknownSuit;
    flushVals_.clear();
    foak_ = Card::unknownValue;
    toak_ = Card::unknownValue;
    toakMissingSuit_ = Card::unknownSuit;
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
                VecValStats::const_iterator itS = stats_.begin();
                itS != stats_.end();
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
        VecValStats::const_iterator itS = stats_.begin();
        itS != stats_.end();
        ++itS
    ) {
        switch(*itS.nCards()) {
        case 4:
            foak_ = itS->value();
            break;
        case 3:
            toak_ = itS->value();
            const VecSuit missingSuits = getMissingSuits(itS->suits());
            #ifdef DSDEBUG
            if (missingSuits.size()) != 1) {
                FatalError << "Expecting only one missing suit from: "
                    << itS->suits() << std::endl;
                abort();
            }
            #endif
            toakMissingCard_ = missingSuits.front();
            break;
        case 2:
            if (pairA_ == Card::unknownValue) {
                pairA_ = itS->value();
                const VecSuit missingSuits = getMissingSuits(itS->suits());
                #ifdef DSDEBUG
                if (missingSuits.size()) != 2) {
                    FatalError << "Expecting two missing suit from: "
                        << itS->suits() << std::endl;
                    abort();
                }
                #endif
                pairAMissingSuits_ =
                    {missingSuits.front(), missingSuits.back()};
            } else {
                pairB_ = itS->value();
                const VecSuit missingSuits = getMissingSuits(itS->suits());
                #ifdef DSDEBUG
                if (missingSuits.size()) != 2) {
                    FatalError << "Expecting two missing suit from: "
                        << itS->suits() << std::endl;
                    abort();
                }
                #endif
                pairBMissingSuits_ =
                    {missingSuits.front(), missingSuits.back()};
            }
        default:
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
