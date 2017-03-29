#include<cctype>
#include<pocketMask.h>

// Static data

#include<pocketMaskDeckIndices.h>
#include<pocketMaskOneCardTables.h>
#include<pocketMaskOneSuitTables.h>
#include<pocketMaskOneValTables.h>
#include<pocketMaskTwoSuitTables.h>
#include<pocketMaskTwoValTables.h>

// *** Constructors *** //

ds::pocketMask::pocketMask():
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{}


ds::pocketMask::pocketMask(short value, suitType suit):
    binValue_(valueToBinValue(value)),
    suit_(suit)
{}


ds::pocketMask::pocketMask(binValueType value, suitType suit):
    binValue_(value),
    suit_(suit)
{}


ds::pocketMask::pocketMask(const char* chStr):
    binValue_(readCharToBinValue(chStr[0])), 
    suit_(readCharToSuit(chStr[1]))
{
    if (chStr[2] != '\0') {
        FatalError << "Invalid pocketMask constructor char array: '" << chStr
            << "'" << std::endl;
        abort();
    }
}


ds::pocketMask::pocketMask(const std::string& str):
    binValue_(readCharToBinValue(str[0])), 
    suit_(readCharToSuit(str[1]))
{
    if (str.size() > 2) {
        FatalError << "Invalid pocketMask constructor string: '" << str
            << "'" << std::endl;
        abort();
    }
}


ds::pocketMask::pocketMask(short di):
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{
    if (di < 0 || di > 51) {
        FatalError << "Attempting to construct a pocketMask from deck index " << di
            << std::endl;
        abort();
    }
    binValue_ = di % 13;
    suit_ = (di-binValue_)/13;
}


ds::pocketMask::pocketMask(std::istream& is):
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{
    is >> *this;
}


// *** Public member functions ***

ds::binValueType ds::pocketMask::readCharToBinValue(char value) {
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


short ds::pocketMask::readCharToValue(char value) {
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


char ds::pocketMask::binValueToWriteChar(binValueType value) {
    if (value >= pocketMask::binTwo && value < binTen) {
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
            FatalError << "Unknown pocketMask value: '" << int(value) << "'"
                << std::endl;
            abort();
    }
}


char ds::pocketMask::valueToWriteChar(short value) {
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
            FatalError << "Unknown pocketMask value: (" << value << ")" << std::endl;
            abort();
    }
}


short ds::pocketMask::binValueToValue(binValueType value) {
    return value + 2;
}


ds::binValueType ds::pocketMask::valueToBinValue(short value) {
    if (value == lowAce) {
        return binAce;
    }
    return value - 2;
}


ds::suitType ds::pocketMask::readCharToSuit(char suit) {
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
    FatalError << "Unknown pocketMask suit: '" << us << "'" << std::endl;
    abort();
}


char ds::pocketMask::suitToWriteChar(suitType suit) {
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
            FatalError << "Unknown pocketMask suit stored internally: ("
                << int(suit) << ")" << std::endl;
            abort();
    }
}


short ds::pocketMask::cardToDeckIndex(const pocketMask& c) {
    if (c.hasWildValue() || c.hasWildSuit() || c.partsUnknown()) {
        return -1;
    }
    return c.binValue() + c.suit()*13;
}


ds::pocketMask ds::pocketMask::cardToDeckIndex(short di) {
    if (di < 0 || di > 51) {
        return pocketMask();
    }
    binValueType value = di % 13;
    suitType suit = (di-value)/13;

    return pocketMask(value, suit);
}


std::ostream& ds::operator<<(std::ostream& os, const pocketMask& c) {
    os << pocketMask::binValueToWriteChar(c.binValue_);
    os << pocketMask::suitToWriteChar(c.suit_);
    return os;
}

std::istream& ds::operator>>(std::istream& in, pocketMask& c) {
    char vc;
    in >> vc;
    c.binValue_ = pocketMask::readCharToBinValue(vc);
    char sc;
    in >> sc;
    c.suit_ = pocketMask::readCharToSuit(sc);
    return in;
}
