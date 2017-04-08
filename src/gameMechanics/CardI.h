namespace ds {

// *** Constructors *** //

Card::Card():
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{}


Card::Card(CardVal value, Suit suit):
    binValue_(valueToBinValue(value)),
    suit_(suit)
{}


Card::Card(BinCardVal value, Suit suit):
    binValue_(value),
    suit_(suit)
{}


Card::Card(const char* chStr):
    binValue_(readCharToBinValue(chStr[0])), 
    suit_(readCharToSuit(chStr[1]))
{
    if (chStr[2] != '\0') {
        FatalError << "Invalid card constructor char array: '" << chStr
            << "'" << std::endl;
        abort();
    }
}


Card::Card(const std::string& str):
    binValue_(readCharToBinValue(str[0])), 
    suit_(readCharToSuit(str[1]))
{
    if (str.size() > 2) {
        FatalError << "Invalid card constructor string: '" << str
            << "'" << std::endl;
        abort();
    }
}


Card::Card(DeckInd di):
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{
    #ifdef DSDEBUG
        if (di < 0 || di > 51) {
            FatalError << "Attempting to construct a card from deck index "
                << di << std::endl;
            abort();
        }
    #endif
    binValue_ = di % 13;
    suit_ = (di-binValue_)/13;
}


Card::Card(std::istream& is):
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{
    is >> *this;
}


// *** Member Functions ***

CardVal Card::binValueToValue(BinCardVal value) {
    return value + 2;
}

BinCardVal Card::valueToBinValue(CardVal value) {
    #ifdef DSDEBUG
    if (value == lowAce) {
        Warning << "lowAce encountered." << std::endl;
        return binAce;
    }
    #endif
    return value - 2;
}


DeckInd Card::cardToDeckIndex(const Card& c) {
    if (c.hasWildValue() || c.hasWildSuit() || c.partsUnknown()) {
        return -1;
    }
    return c.binValue() + c.suit()*13;
}


Card Card::deckIndexToCard(DeckInd di) {
    if (di < 0 || di > 51) {
        return Card();
    }
    BinCardVal value = di % 13;
    Suit suit = (di-value)/13;

    return Card(value, suit);
}


CardVal Card::value() const {
    return binValueToValue(binValue_);
}


BinCardVal Card::binValue() const {
    return binValue_;
}


Suit Card::suit() const {
    return suit_;
}


bool Card::hasWildValue() const {
    return binValue_ == binWildValue;
}


bool Card::hasWildSuit() const {
    return suit_ == wildSuit;
}


bool Card::hasWild() const {
    return hasWildSuit() || hasWildValue();
}


bool Card::wild() const {
    return hasWildSuit() && hasWildValue();
}


bool Card::valid() const {
    return 
        binValue_ <= 13 && binValue_ >= 0
     && suit_  <= 4 && suit_ >= 0;
}

bool Card::real() const {
    return
        binValue_ <= 13 && binValue_ >= 0
     && suit_  <= 3 && suit_ >= 0;
}


bool Card::partsUnknown() const {
    return binValue_ == binUnknownValue || suit_ == unknownSuit;
}


ds::DeckInd ds::Card::deckIndex() const {
    return cardToDeckIndex(*this);
}


// *** Member Operators *** //

bool Card::operator<(const Card& c1) const {
    return binValue_ < c1.binValue_;
}


bool Card::operator<=(const Card& c1) const {
    if (c1.binValue_ == binWildValue || binValue_ == binWildValue) {
        return true;
    }
    return binValue_ <= c1.binValue_;
}
        

bool Card::operator>(const Card& c1) const {
    return binValue_ > c1.binValue_;
}


bool Card::operator>=(const Card& c1) const {
    if (c1.binValue_ == binWildValue || binValue_ == binWildValue) {
        return true;
    }
    return binValue_ >= c1.binValue_;
}

        
bool Card::operator==(const Card& c1) const {
    return
        (
            (c1.binValue_ == binValue_)
         || (c1.binValue_ == binWildValue)
         || (binValue_ == binWildValue)
        ) && (
            (c1.suit_ == suit_)
         || (c1.suit_ == wildSuit)
         || (suit_ == wildSuit)
        );
}


bool Card::operator!=(const Card& c1) const {
    return !(operator==(c1));
}

} // end namespace ds
