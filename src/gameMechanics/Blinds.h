#ifndef Blinds_h
#define Blinds_h

#include<types.h>;

namespace ds {

// Forward declarations

struct Blinds;
std::ostream& operator<<(std::ostream& os, const Blinds& c);
std::istream& operator>>(std::istream& in, Blinds& c);


struct Blinds {

    // Public data
    Money ante;
    Money smallBlind;
    Money bigBlind;
    
    
    // Constructors
    
        //- Construct null
        Blinds();
        
        //- Construct from components
        Blinds(Money ant, Money sb, Money bb);

    
    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const Blinds& c);
    friend std::istream& operator>>(std::istream& in, Blinds& c);
};


} // end namespace ds

#endif
