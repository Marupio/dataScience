#ifndef Suit_h
#define Suit_h

#include<vector.h>
#include<array>

namespace ds {

// Typedefs

    typedef signed char Suit;
    typedef std::vector<Suit> VecSuit;
    typedef std::vector<VecSuit> VecVecSuit;
    typedef std::array<char, 4> SuitCount;
    typedef std::vector<SuitCount> VecSuitCount;

// Global functions

    void writeVecSuit(const VecSuit& vs, std::ostream& os);
    VecSuit readVecSuit(std::istream& is);

    void writeVecVecSuit(const VecVecSuit& vs, std::ostream& os);
    VecVecSuit readVecVecSuit(std::istream& is);

    void writeSuitCount(const SuitCount& sc, std::ostream& os);
    SuitCount readSuitCount(std::istream& is);

    void writeVecSuitCount(const VecSuitCount& vsc, std::ostream& os);
    VecSuitCount readVecSuitCount(std::istream& is);
} // end namespace ds

#endif
