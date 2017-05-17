#include <Blinds.h>
#include <vector.h>

// ****** Constructors ****** //

ds::Blinds::Blinds():
    ante(0),
    smallBlind(0),
    bigBlind(0),
    startTime(0)
{}


ds::Blinds::Blinds(Money ant, Money sb, Money bb, int st):
    ante(ant),
    smallBlind(sb),
    bigBlind(bb),
    startTime(st)
{}


// ****** Global operators ****** //

std::ostream& ds::operator<<(std::ostream& os, const Blinds& c) {
    os << "(" << c.ante << " " << c.smallBlind << " " << c.bigBlind << ")";
    return os;
}


std::istream& ds::operator>>(std::istream& is, Blinds& c) {
    std::vector<float> tmp;
    is >> tmp;
    if (tmp.size() != 3) {
        FatalError << "Expecting three values for blinds. Got " << tmp.size()
            << ". Read values are: " << tmp << std::endl;
        abort();
    }
    auto it = tmp.cbegin();
    c.ante = *(it++);
    c.smallBlind = *(it++);
    c.bigBlind = *(it++);
    return is;
}

// ****** END ****** //
