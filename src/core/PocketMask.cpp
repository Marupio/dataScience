#include<cctype>
#include<PocketMask.h>

// Static data

#include<PocketMaskDeckIndices.h>
#include<PocketMaskOneCardTables.h>
#include<PocketMaskOneSuitTables.h>
#include<PocketMaskOneValTables.h>
#include<PocketMaskTwoSuitTables.h>
#include<PocketMaskTwoValTables.h>


/////////////////////////////
// *** Constructors *** //

ds::PocketMask::PocketMask():
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{}


ds::PocketMask::PocketMask(short value, suitType suit):
    binValue_(valueToBinValue(value)),
    suit_(suit)
{}


ds::PocketMask::PocketMask(binValueType value, suitType suit):
    binValue_(value),
    suit_(suit)
{}


ds::PocketMask::PocketMask(const char* chStr):
    binValue_(readCharToBinValue(chStr[0])), 
    suit_(readCharToSuit(chStr[1]))
{
    if (chStr[2] != '\0') {
        FatalError << "Invalid PocketMask constructor char array: '" << chStr
            << "'" << std::endl;
        abort();
    }
}


ds::PocketMask::PocketMask(const std::string& str):
    binValue_(readCharToBinValue(str[0])), 
    suit_(readCharToSuit(str[1]))
{
    if (str.size() > 2) {
        FatalError << "Invalid PocketMask constructor string: '" << str
            << "'" << std::endl;
        abort();
    }
}


ds::PocketMask::PocketMask(short di):
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{
    if (di < 0 || di > 51) {
        FatalError << "Attempting to construct a PocketMask from deck index " << di
            << std::endl;
        abort();
    }
    binValue_ = di % 13;
    suit_ = (di-binValue_)/13;
}


ds::PocketMask::PocketMask(std::istream& is):
    binValue_(binUnknownValue),
    suit_(unknownSuit)
{
    is >> *this;
}


// *** Public member functions ***

ds::binValueType ds::PocketMask::readCharToBinValue(char value) {
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


short ds::PocketMask::readCharToValue(char value) {
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


char ds::PocketMask::binValueToWriteChar(binValueType value) {
    if (value >= PocketMask::binTwo && value < binTen) {
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
            FatalError << "Unknown PocketMask value: '" << int(value) << "'"
                << std::endl;
            abort();
    }
}


char ds::PocketMask::valueToWriteChar(short value) {
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
            FatalError << "Unknown PocketMask value: (" << value << ")" << std::endl;
            abort();
    }
}


short ds::PocketMask::binValueToValue(binValueType value) {
    return value + 2;
}


ds::binValueType ds::PocketMask::valueToBinValue(short value) {
    if (value == lowAce) {
        return binAce;
    }
    return value - 2;
}


ds::suitType ds::PocketMask::readCharToSuit(char suit) {
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
    FatalError << "Unknown PocketMask suit: '" << us << "'" << std::endl;
    abort();
}


char ds::PocketMask::suitToWriteChar(suitType suit) {
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
            FatalError << "Unknown PocketMask suit stored internally: ("
                << int(suit) << ")" << std::endl;
            abort();
    }
}


short ds::PocketMask::cardToDeckIndex(const PocketMask& c) {
    if (c.hasWildValue() || c.hasWildSuit() || c.partsUnknown()) {
        return -1;
    }
    return c.binValue() + c.suit()*13;
}


ds::PocketMask ds::PocketMask::cardToDeckIndex(short di) {
    if (di < 0 || di > 51) {
        return PocketMask();
    }
    binValueType value = di % 13;
    suitType suit = (di-value)/13;

    return PocketMask(value, suit);
}


std::ostream& ds::operator<<(std::ostream& os, const PocketMask& c) {
    os << PocketMask::binValueToWriteChar(c.binValue_);
    os << PocketMask::suitToWriteChar(c.suit_);
    return os;
}

std::istream& ds::operator>>(std::istream& in, PocketMask& c) {
    char vc;
    in >> vc;
    c.binValue_ = PocketMask::readCharToBinValue(vc);
    char sc;
    in >> sc;
    c.suit_ = PocketMask::readCharToSuit(sc);
    return in;
}
