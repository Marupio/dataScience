#include<cctype>
#include<Card.h>


// ****** Static data ****** //

const ds::Card wildCard(Card::wildValue, Card::wildSuit);

const short ds::Card::nSuits(4);

const ds::Suit ds::Card::unknownSuit(-1);
const ds::Suit ds::Card::clubs(0);
const ds::Suit ds::Card::diamonds(1);
const ds::Suit ds::Card::hearts(2);
const ds::Suit ds::Card::spades(3);
const ds::Suit ds::Card::wildSuit(4);

const ds::BinCardVal ds::Card::binUnknownValue(-1);
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


// ****** Public member functions ******

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
