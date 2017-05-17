#include <algorithm>
#include <ValStats.h>

// ****** Constructors ****** //

ds::ValStats::ValStats():
    std::tuple<ds::CardVal, short, ds::SuitCount>(
        Card::unknownValue,
        0,
        {0, 0, 0, 0}
    ) {}


ds::ValStats::ValStats(CardVal value, short nCards, SuitCount& suits):
    std::tuple<ds::CardVal, short, ds::SuitCount>(
        value,
        nCards,
        suits
    ) {}


// ****** Member Functions ****** //
    
ds::CardVal& ds::ValStats::value() {
    return std::get<0>(*this);
}


ds::CardVal ds::ValStats::value() const {
    return std::get<0>(*this);
}


short& ds::ValStats::nCards() {
    return std::get<1>(*this);
}


short ds::ValStats::nCards() const {
    return std::get<1>(*this);
}


ds::SuitCount& ds::ValStats::suits() {
    return std::get<2>(*this);
}


const ds::SuitCount& ds::ValStats::suits() const {
    return std::get<2>(*this);
}


// ****** Global operators ****** //

std::ostream& ds::operator<<(std::ostream& os, const ValStats& vs) {
    os << "{" << Card::valueToWriteChar(std::get<0>(vs)) << " "
        << std::get<1>(vs) << " ";
    writeSuitCount(std::get<2>(vs), os);
    os << "}";
    return os;
}

std::istream& ds::operator>>(std::istream& in, ValStats& vs) {
    char vc;
    in >> vc;
    if (vc != '{') {
        FatalError << "Expecting '{' when reading ValStats. Got '"
            << vc << "', which is int " << short(vc) << "." << std::endl;
        abort();
    }
    in >> std::get<0>(vs);
    in >> std::get<1>(vs);
    std::get<2>(vs) = readSuitCount(in);
    in >> vc;
    if (vc != '}') {
        FatalError << "Expecting '}' when reading ValStats. Got '"
            << vc << "', which is int " << short(vc) << "." << std::endl;
        abort();
    }
    return in;
}

// ****** END ****** //
