#include<PocketCards.h>

// *** Member operators *** //

std::ostream& ds::operator<<(std::ostream& os, const PocketCards& c) {
    os << first << " " << second;
    return os;
}

std::istream& ds::operator>>(std::istream& in, PocketCards& c) {
    in >> first;
    in >> second;
    return in;
}
