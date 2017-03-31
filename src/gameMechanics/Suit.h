#ifndef Suit_h
#define Suit_h

#include<vector.h>

namespace ds {

// Typedefs

    typedef signed char Suit;
    typedef std::vector<Suit> VecSuit;
    typedef std::vector<VecSuit> VecVecSuit;

// Global functions

    void writeVecSuit(const VecSuit& vs, std::ostream& os);
    VecSuit readVecSuit(std::istream& is);

    void writeVecVecSuit(const VecVecSuit& vs, std::ostream& os);
    VecVecSuit readVecVecSuit(std::istream& is);

} // end namespace ds

#endif
