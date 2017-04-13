#include<cctype>
#include<Card.h>


// ****** Static data ****** //

const ds::Card ds::Card::unknownCard(Card::unknownValue, Card::unknownSuit);
const ds::Card ds::Card::wildCard(Card::wildValue, Card::wildSuit);

const short ds::Card::nSuits(4);

const ds::Suit ds::Card::unknownSuit(-1);
const ds::Suit ds::Card::clubs(0);
const ds::Suit ds::Card::diamonds(1);
const ds::Suit ds::Card::hearts(2);
const ds::Suit ds::Card::spades(3);
const ds::Suit ds::Card::wildSuit(4);

const ds::BinCardVal ds::Card::binUnknownValue(-2);
const ds::BinCardVal ds::Card::binLowAce(-1);
const ds::BinCardVal ds::Card::binTwo(0);
const ds::BinCardVal ds::Card::binThree(1);
const ds::BinCardVal ds::Card::binFour(2);
const ds::BinCardVal ds::Card::binFive(3);
const ds::BinCardVal ds::Card::binSix(4);
const ds::BinCardVal ds::Card::binSeven(5);
const ds::BinCardVal ds::Card::binEight(6);
const ds::BinCardVal ds::Card::binNine(7);
const ds::BinCardVal ds::Card::binTen(8);
const ds::BinCardVal ds::Card::binJack(9);
const ds::BinCardVal ds::Card::binQueen(10);
const ds::BinCardVal ds::Card::binKing(11);
const ds::BinCardVal ds::Card::binAce(12);
const ds::BinCardVal ds::Card::binWildValue(13);

const ds::CardVal ds::Card::unknownValue(0);
const ds::CardVal ds::Card::lowAce(1);
const ds::CardVal ds::Card::two(2);
const ds::CardVal ds::Card::three(3);
const ds::CardVal ds::Card::four(4);
const ds::CardVal ds::Card::five(5);
const ds::CardVal ds::Card::six(6);
const ds::CardVal ds::Card::seven(7);
const ds::CardVal ds::Card::eight(8);
const ds::CardVal ds::Card::nine(9);
const ds::CardVal ds::Card::ten(10);
const ds::CardVal ds::Card::jack(11);
const ds::CardVal ds::Card::queen(12);
const ds::CardVal ds::Card::king(13);
const ds::CardVal ds::Card::ace(14);
const ds::CardVal ds::Card::wildValue(15);


// *** Constructors *** //

ds::Card::Card():
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{}


ds::Card::Card(CardVal value, Suit suit):
    binValue_(valueToBinValue(value)),
    suit_(suit)
{}


ds::Card::Card(BinCardVal value, Suit suit):
    binValue_(value),
    suit_(suit)
{}


ds::Card::Card(const char* chStr):
    binValue_(readCharToBinValue(chStr[0])), 
    suit_(readCharToSuit(chStr[1]))
{
    if (chStr[2] != '\0') {
        FatalError << "Invalid card constructor char array: '" << chStr
            << "'" << std::endl;
        abort();
    }
}


ds::Card::Card(const std::string& str):
    binValue_(readCharToBinValue(str[0])), 
    suit_(readCharToSuit(str[1]))
{
    if (str.size() > 2) {
        FatalError << "Invalid card constructor string: '" << str
            << "'" << std::endl;
        abort();
    }
}


ds::Card::Card(DeckInd di):
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


ds::Card::Card(std::istream& is):
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{
    is >> *this;
}


// ****** Public Member Functions ******

ds::BinCardVal ds::Card::readCharToBinValue(char value) {
    if (value > 49 && value < 58) {
        return value - 50;
    }
    if (value == '?') {
        return binUnknownValue;
    }
    if (value == '*') {
        return binWildValue;
    }
    char uv(std::toupper(value));
    if (uv == 'T') {
        return binTen;
    }
    if (uv == 'J') {
        return binJack;
    }
    if (uv == 'Q') {
        return binQueen;
    }
    if (uv == 'K') {
        return binKing;
    }
    if (uv == 'A') {
        return binAce;
    }
    FatalError << "Read unknown BinCardValue: '" << value << "', which is "
        << "int " << int(value) << std::endl;
    abort();
    return binUnknownValue;
}


ds::CardVal ds::Card::readCharToValue(char value) {
    if (value > 49 && value < 58) {
        return value - 48;
    }
    if (value == '?') {
        return unknownValue;
    }
    if (value == '*') {
        return wildValue;
    }
    char uv(std::toupper(value));
    if (uv == 'T') {
        return ten;
    }
    if (uv == 'J') {
        return jack;
    }
    if (uv == 'Q') {
        return queen;
    }
    if (uv == 'K') {
        return king;
    }
    if (uv == 'A') {
        return ace;
    }
    FatalError << "Read unknown CardValue: '" << value << "', which is "
        << "int " << int(value) << std::endl;
    abort();
    return unknownValue;
}


char ds::Card::binValueToWriteChar(BinCardVal value) {
    if (value >= Card::binTwo && value < binTen) {
        return value + 50;
    }
    switch (value)
    {
        case binUnknownValue:
            return '?';
        case binTen:
            return 'T';
        case binJack:
            return 'J';
        case binQueen:
            return 'Q';
        case binKing:
            return 'K';
        case binLowAce: // fall through
        case binAce:
            return 'A';
        case binWildValue:
            return '*';
        default:
            FatalError << "Unknown card value: '" << int(value) << "'"
                << std::endl;
            abort();
    }
}


char ds::Card::valueToWriteChar(CardVal value) {
    if (value > lowAce && value < ten) {
        return value + 48;
    }
    switch (value)
    {
        case unknownValue:
            return '?';
        case lowAce:
            return 'A';
        case ten:
            return 'T';
        case jack:
            return 'J';
        case queen:
            return 'Q';
        case king:
            return 'K';
        case ace:
            return 'A';
        case wildValue:
            return '*';
        default:
            FatalError << "Unknown card value: (" << value << ")" << std::endl;
            abort();
    }
}


ds::CardVal ds::Card::binValueToValue(BinCardVal value) {
    return value + 2;
}


ds::BinCardVal ds::Card::valueToBinValue(CardVal value) {
    #ifdef DSDEBUG
    if (value == lowAce) {
        Warning << "lowAce encountered." << std::endl;
        return binAce;
    }
    #endif
    return value - 2;
}


ds::Suit ds::Card::readCharToSuit(char suit) {
    char us(std::toupper(suit));
    if (us == '?') {
        return unknownSuit;
    }
    if (us == '*') {
        return wildSuit;
    }
    if (us == 'C') {
        return clubs;
    }
    if (us == 'D') {
        return diamonds;
    }
    if (us == 'H') {
        return hearts;
    }
    if (us == 'S') {
        return spades;
    }
    FatalError << "Unknown card suit: '" << us << "'" << std::endl;
    abort();
}


char ds::Card::suitToWriteChar(Suit suit) {
    switch (suit)
    {
        case unknownSuit:
            return '?';
        case wildSuit:
            return '*';
        case clubs:
            return 'c';
        case diamonds:
            return 'd';
        case hearts:
            return 'h';
        case spades:
            return 's';
        default:
            FatalError << "Unknown card suit stored internally: ("
                << int(suit) << ")" << std::endl;
            abort();
    }
}


ds::DeckInd ds::Card::cardToDeckIndex(const Card& c) {
    if (c.hasWildValue() || c.hasWildSuit() || c.partsUnknown()) {
        return -1;
    }
    return c.binValue() + c.suit()*13;
}


ds::Card ds::Card::deckIndexToCard(DeckInd di) {
    if (di < 0 || di > 51) {
        return Card();
    }
    BinCardVal value = di % 13;
    Suit suit = (di-value)/13;

    return Card(value, suit);
}


ds::CardVal ds::Card::value() const {
    return binValueToValue(binValue_);
}


ds::BinCardVal ds::Card::binValue() const {
    return binValue_;
}


ds::Suit ds::Card::suit() const {
    return suit_;
}


bool ds::Card::hasWildValue() const {
    return binValue_ == binWildValue;
}


bool ds::Card::hasWildSuit() const {
    return suit_ == wildSuit;
}


bool ds::Card::hasWild() const {
    return hasWildSuit() || hasWildValue();
}


bool ds::Card::wild() const {
    return hasWildSuit() && hasWildValue();
}


bool ds::Card::valid() const {
    return 
        binValue_ <= 13 && binValue_ >= 0
     && suit_  <= 4 && suit_ >= 0;
}

bool ds::Card::real() const {
    return
        binValue_ <= 13 && binValue_ >= 0
     && suit_  <= 3 && suit_ >= 0;
}


bool ds::Card::partsUnknown() const {
    return binValue_ == binUnknownValue || suit_ == unknownSuit;
}


ds::DeckInd ds::Card::deckIndex() const {
    return cardToDeckIndex(*this);
}


// ****** Member Operators ****** //

bool ds::Card::operator<(const Card& c1) const {
    return binValue_ < c1.binValue_;
}


bool ds::Card::operator<=(const Card& c1) const {
    if (c1.binValue_ == binWildValue || binValue_ == binWildValue) {
        return true;
    }
    return binValue_ <= c1.binValue_;
}


bool ds::Card::operator>(const Card& c1) const {
    return binValue_ > c1.binValue_;
}


bool ds::Card::operator>=(const Card& c1) const {
    if (c1.binValue_ == binWildValue || binValue_ == binWildValue) {
        return true;
    }
    return binValue_ >= c1.binValue_;
}

        
bool ds::Card::operator==(const Card& c1) const {
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


bool ds::Card::operator!=(const Card& c1) const {
    return !(operator==(c1));
}


// ****** Global operators ****** //

std::ostream& ds::operator<<(std::ostream& os, const Card& c) {
    os << Card::binValueToWriteChar(c.binValue_);
    os << Card::suitToWriteChar(c.suit_);
    return os;
}

std::istream& ds::operator>>(std::istream& in, Card& c) {
    char vc;
    in >> vc;
    c.binValue_ = Card::readCharToBinValue(vc);
    char sc;
    in >> sc;
    c.suit_ = Card::readCharToSuit(sc);
    return in;
}


// ****** Global Functions ****** //

bool ds::noWildEquals(const Card& cA, const Card& cB) {
    #ifdef DSDEBUG
    if (cA.hasWild() || cB.hasWild()) {
        FatalError << "Attempting to use non-wild comparison on cards with "
            << "wilds.  Cards are: " << cA << " " << cB << std::endl;
        abort();
    }
    #endif
    return (
        cA.binValue() == cB.binValue()
     && cA.suit() == cB.suit()
    );
}

