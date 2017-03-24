#include<cctype>
#include<card.h>
#include<error.h>

short ds::card::charToValue(char value) {
    if (value > 49 && value < 58) {
        return value - 48;
    }
    if (value == '?') {
        return 0;
    }
    if (value == '*') {
        return 1;
    }
    char uv(std::toupper(value));
    if (uv == 'T') {
        return 10;
    }
    if (uv == 'J') {
        return 11;
    }
    if (uv == 'Q') {
        return 12;
    }
    if (uv == 'K') {
        return 13;
    }
    if (uv == 'A') {
        return 14;
    }
}



char ds::card::valueToChar(short value) {
    if (value > 1 && value < 10) {
        return value + 48;
    }
    switch (value)
    {
        case 0:
            return '?';
        case 1:
            return '*';
        case 10:
            return 'T';
        case 11:
            return 'J';
        case 12:
            return 'Q';
        case 13:
            return 'K';
        case 14:
            return 'A';
        default:
            FatalError << "Error! Unknown card value: " << value << std::endl;
            abort();
    }
}


ds::card::suitEnum ds::card::charToSuit(char suit) {
    char us(std::toupper(suit));
    if (us == '?') {
        return UNKNOWN;
    }
    if (us == '*') {
        return WILD;
    }
    if (us == 'C') {
        return CLUBS;
    }
    if (us == 'D') {
        return DIAMONDS;
    }
    if (us == 'H') {
        return HEARTS;
    }
    if (us == 'S') {
        return SPADES;
    }
    FatalError << "Error! Unknown card suit: " << us << std::endl;
    abort();
}


char ds::card::suitToChar(suitEnum suit) {
    switch (suit)
    {
        case UNKNOWN:
            return '?';
        case WILD:
            return '*';
        case CLUBS:
            return 'C';
        case DIAMONDS:
            return 'D';
        case HEARTS:
            return 'H';
        case SPADES:
            return 'S';
        default:
            FatalError << "Error! Unknown card suit: " << suit << std::endl;
            abort();
    }
}


std::ostream& ds::operator<<(std::ostream& os, const card& c) {
    os << card::valueToChar(c.value_);
    os << card::suitToChar(c.suit_);
    return os;
}

std::istream& ds::operator>>(std::istream& in, card& c) {
    char vc;
    in >> vc;
    short v = card::charToValue(vc);
    c.value_ = v;
    char sc;
    in >> sc;
    card::suitEnum s = card::charToSuit(sc);
    c.suit_ = s;
    return in;
}
