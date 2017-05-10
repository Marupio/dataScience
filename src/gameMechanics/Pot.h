// A set of typedefs for building a hierarchical pot structure to account for
// multiple side pots

#ifndef Pot_h
#define Pot_h

#include<Player.h>
#include<algorithm>

namespace ds {

// type definitions

// PushedMoney represents money in front of a player, across the commit line
// This is then searched for duplicate all-in amounts and reduced to Pot type

// Forward declaration

struct PushedMoney;
std::ostream& operator<<(std::ostream& os, const PushedMoney& pm);

struct PushedMoney:
    public std::pair<Money, SeatedPlayer>
{
    PushedMoney(Money amount, SeatedPlayer& sp):
        std::pair<Money, SeatedPlayer>(amount, sp)
    {}

    bool operator<(const PushedMoney& pm) const {
        return first < pm.first;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const PushedMoney& pm);
};


struct VecPushedMoney:
    public std::vector<PushedMoney>
{
    std::vector<PushedMoney>::iterator find(const SeatedPlayer& player);
};


// Money is the amount in this pot, VecSeatedPlayer are all the players vying
// for this pot. Duplicates occur when two players are all in with the same
// amount, and the main pot for players who are not all in

// Forward declarations
struct Pot;
std::ostream& operator<<(std::ostream& os, const Pot& pt);


struct Pot:
    public std::pair<Money, VecSeatedPlayer>
{
    //- True if pot contains an all-in player and cannot be given more Money
    bool sealed;

    //- Construct null    
    Pot():
        std::pair<Money, VecSeatedPlayer>(0, VecSeatedPlayer(0)),
        sealed(false)
    {}

    //- Construct from components
    Pot(Money amount, VecSeatedPlayer&& vs):
        std::pair<Money, VecSeatedPlayer>(amount, vs),
        sealed(false)
    {}
    
    bool operator<(const Pot& pt) const {
        return first < pt.first;
    }

    friend std::ostream& operator<<(std::ostream& os, const Pot& pt);
};


struct VecPot:
    public std::vector<Pot>
{
    void collect(VecPushedMoney& pm);
};

} // end namespace ds

#endif
