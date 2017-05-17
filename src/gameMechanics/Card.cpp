#include <cctype>
#include <Card.h>


// ****** Static data ****** //

const ds::Card ds::Card::unknownCard(Card::unknownValue, Card::unknownSuit);
const ds::Card ds::Card::wildCard(Card::wildValue, Card::wildSuit);

const short ds::Card::nSuits(4);

const ds::Suit ds::Card::unknownSuit(-2);
const ds::Suit ds::Card::wildSuit(-1);
const ds::Suit ds::Card::clubs(0);
const ds::Suit ds::Card::diamonds(1);
const ds::Suit ds::Card::hearts(2);
const ds::Suit ds::Card::spades(3);

const ds::CardVal ds::Card::unknownValue(-1);
const ds::CardVal ds::Card::wildValue(0);
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


const std::array<std::string, 15> ds::Card::CardValNames = {
    "WildValue",
    "LowAce",
    "Two",
    "Three",
    "Four",
    "Five",
    "Six",
    "Seven",
    "Eight",
    "Nine",
    "Ten",
    "Jack",
    "Queen",
    "King",
    "Ace"
};

const std::array<std::string, 15> ds::Card::CardValNamesPlural = {
    "WildValues",
    "LowAces",
    "Twos",
    "Threes",
    "Fours",
    "Fives",
    "Sixes",
    "Sevens",
    "Eights",
    "Nines",
    "Tens",
    "Jacks",
    "Queens",
    "Kings",
    "Aces"
};


const std::array<std::string, 4> ds::Card::CardSuitNames = {
    "Clubs",
    "Diamonds",
    "Hearts",
    "Spades"
};


// *** Constructors *** //

ds::Card::Card():
    value_(unknownValue),
    suit_(unknownSuit)
{}


ds::Card::Card(CardVal value, Suit suit):
    value_(value),
    suit_(suit)
{}


ds::Card::Card(const char* chStr):
    value_(readCharToValue(chStr[0])), 
    suit_(readCharToSuit(chStr[1]))
{
    if (chStr[2] != '\0') {
        FatalError << "Invalid card constructor char array: '" << chStr
            << "'" << std::endl;
        abort();
    }
}


ds::Card::Card(const std::string& str):
    value_(readCharToValue(str[0])), 
    suit_(readCharToSuit(str[1]))
{
    if (str.size() > 2) {
        FatalError << "Invalid card constructor string: '" << str
            << "'" << std::endl;
        abort();
    }
}


ds::Card::Card(DeckInd di):
    value_(unknownValue),
    suit_(unknownSuit)
{
    #ifdef DSDEBUG
        if (di < 0 || di > 51) {
            FatalError << "Attempting to construct a card from deck index "
                << di << std::endl;
            abort();
        }
    #endif
    value_ = di%13 + 2;
    suit_ = (di - value_ + 2)/13;
}


ds::Card::Card(std::istream& is):
    value_(unknownValue),
    suit_(unknownSuit)
{
    is >> *this;
}


// ****** Public Member Functions ******

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
    return c.value() - 2 + c.suit()*13;
}


ds::Card ds::Card::deckIndexToCard(DeckInd di) {
    if (di < 0 || di > 51) {
        return Card();
    }
    CardVal value = di%13 + 2;
    Suit suit = (di - value + 2)/13;

    return Card(value, suit);
}


ds::CardVal ds::Card::value() const {
    return value_;
}


ds::Suit ds::Card::suit() const {
    return suit_;
}


bool ds::Card::hasWildValue() const {
    return value_ == wildValue;
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
        value_ <= ace && value_ >= wildValue
     && suit_  <= 3 && suit_ >= 0;
}


bool ds::Card::real() const {
    return
        value_ <= ace && value_ >= two
     && suit_  <= 3 && suit_ >= 0;
}


bool ds::Card::partsUnknown() const {
    return value_ == unknownValue || suit_ == unknownSuit;
}


ds::DeckInd ds::Card::deckIndex() const {
    return cardToDeckIndex(*this);
}


// ****** Member Operators ****** //

bool ds::Card::operator<(const Card& c1) const {
    return value_ < c1.value_;
}


bool ds::Card::operator<=(const Card& c1) const {
    if (c1.value_ == wildValue || value_ == wildValue) {
        return true;
    }
    return value_ <= c1.value_;
}


bool ds::Card::operator>(const Card& c1) const {
    return value_ > c1.value_;
}


bool ds::Card::operator>=(const Card& c1) const {
    if (c1.value_ == wildValue || value_ == wildValue) {
        return true;
    }
    return value_ >= c1.value_;
}

        
bool ds::Card::operator==(const Card& c1) const {
    return
        (
            (c1.value_ == value_)
         || (c1.value_ == wildValue)
         || (value_ == wildValue)
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
    os << Card::valueToWriteChar(c.value_);
    os << Card::suitToWriteChar(c.suit_);
    return os;
}

std::istream& ds::operator>>(std::istream& in, Card& c) {
    char vc;
    in >> vc;
    c.value_ = Card::readCharToValue(vc);
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
        cA.value() == cB.value()
     && cA.suit() == cB.suit()
    );
}

