#include<Blinds.h>
#include<vector.h>

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
    startTime(startTime)
{}


// ****** Global operators ****** //

std::ostream& ds::operator<<(std::ostream& out, const Blinds& c) {
    os << "(" << ante << " " << smallBlind << " " << bigBlind << ")";
}


std::istream& ds::operator>>(std::istream& in, Blinds& c) {
    std::vector<float> tmp;
    is >> tmp;
    if (tmp.size() != 3) {
        FatalError << "Expecting three values for blinds. Got " << tmp.size()
            << ". Read values are: " << tmp << std::endl;
        abort();
    }
    auto it = tmp.cbegin();
    ante = *(it++);
    smallBlind = *(it++);
    bigBlind = *(it++);
}

// ****** END ****** //
