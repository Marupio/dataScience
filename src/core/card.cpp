#include<cctype>
#include<card.h>

// Static data

const ds::suitType ds::card::unknownSuit(-1);
const ds::suitType ds::card::clubs(0);
const ds::suitType ds::card::diamonds(1);
const ds::suitType ds::card::hearts(2);
const ds::suitType ds::card::spades(3);
const ds::suitType ds::card::wildSuit(4);

const ds::binValueType ds::card::binUnknownValue(-1);
const ds::binValueType ds::card::binTwo(0);
const ds::binValueType ds::card::binThree(1);
const ds::binValueType ds::card::binFour(2);
const ds::binValueType ds::card::binFive(3);
const ds::binValueType ds::card::binSix(4);
const ds::binValueType ds::card::binSeven(5);
const ds::binValueType ds::card::binEight(6);
const ds::binValueType ds::card::binNine(7);
const ds::binValueType ds::card::binTen(8);
const ds::binValueType ds::card::binJack(9);
const ds::binValueType ds::card::binQueen(10);
const ds::binValueType ds::card::binKing(11);
const ds::binValueType ds::card::binAce(12);
const ds::binValueType ds::card::binWildValue(13);

const short ds::card::unknownValue(0);
const short ds::card::lowAce(1);
const short ds::card::two(2);
const short ds::card::three(3);
const short ds::card::four(4);
const short ds::card::five(5);
const short ds::card::six(6);
const short ds::card::seven(7);
const short ds::card::eight(8);
const short ds::card::nine(9);
const short ds::card::ten(10);
const short ds::card::jack(11);
const short ds::card::queen(12);
const short ds::card::king(13);
const short ds::card::ace(14);
const short ds::card::wildValue(15);


// *** Constructors *** //

ds::card::card():
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{}


ds::card::card(short value, suitType suit):
    binValue_(valueToBinValue(value)),
    suit_(suit)
{}


ds::card::card(binValueType value, suitType suit):
    binValue_(value),
    suit_(suit)
{}


ds::card::card(const char* chStr):
    binValue_(readCharToBinValue(chStr[0])), 
    suit_(readCharToSuit(chStr[1]))
{
    if (chStr[2] != '\0') {
        FatalError << "Invalid card constructor char array: '" << chStr
            << "'" << std::endl;
        abort();
    }
}


ds::card::card(const std::string& str):
    binValue_(readCharToBinValue(str[0])), 
    suit_(readCharToSuit(str[1]))
{
    if (str.size() > 2) {
        FatalError << "Invalid card constructor string: '" << str
            << "'" << std::endl;
        abort();
    }
}


ds::card::card(short di):
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{
    if (di < 0 || di > 51) {
        FatalError << "Attempting to construct a card from deck index " << di
            << std::endl;
        abort();
    }
    binValue_ = di % 13;
    suit_ = (di-binValue_)/13;
}


ds::card::card(std::istream& is):
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{
    is >> *this;
}


// *** Public member functions ***

ds::binValueType ds::card::readCharToBinValue(char value) {
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
}


short ds::card::readCharToValue(char value) {
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
}


char ds::card::binValueToWriteChar(binValueType value) {
    if (value >= card::binTwo && value < binTen) {
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


char ds::card::valueToWriteChar(short value) {
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


short ds::card::binValueToValue(binValueType value) {
    return value + 2;
}


ds::binValueType ds::card::valueToBinValue(short value) {
    if (value == lowAce) {
        return binAce;
    }
    return value - 2;
}


ds::suitType ds::card::readCharToSuit(char suit) {
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


char ds::card::suitToWriteChar(suitType suit) {
    switch (suit)
    {
        case unknownSuit:
            return '?';
        case wildSuit:
            return '*';
        case clubs:
            return 'C';
        case diamonds:
            return 'D';
        case hearts:
            return 'H';
        case spades:
            return 'S';
        default:
            FatalError << "Unknown card suit stored internally: ("
                << int(suit) << ")" << std::endl;
            abort();
    }
}


short ds::card::cardToDeckIndex(const card& c) {
    if (c.hasWildValue() || c.hasWildSuit() || c.partsUnknown()) {
        return -1;
    }
    return c.binValue() + c.suit()*13;
}


ds::card ds::card::cardToDeckIndex(short di) {
    if (di < 0 || di > 51) {
        return card();
    }
    binValueType value = di % 13;
    suitType suit = (di-value)/13;

    return card(value, suit);
}


std::ostream& ds::operator<<(std::ostream& os, const card& c) {
    os << card::binValueToWriteChar(c.binValue_);
    os << card::suitToWriteChar(c.suit_);
    return os;
}

std::istream& ds::operator>>(std::istream& in, card& c) {
    char vc;
    in >> vc;
    c.binValue_ = card::readCharToBinValue(vc);
    char sc;
    in >> sc;
    c.suit_ = card::readCharToSuit(sc);
    return in;
}
