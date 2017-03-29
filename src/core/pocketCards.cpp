#include<pocketCards.h>

// *** Member operators *** //

std::ostream& ds::operator<<(std::ostream& os, const pocketCards& c) {
    os << first << " " << second;
    return os;
}

std::istream& ds::operator>>(std::istream& in, pocketCards& c) {
    in >> first;
    in >> second;
    return in;
}
