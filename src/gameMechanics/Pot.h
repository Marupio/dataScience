// A set of typedefs for building a hierarchical pot structure to account for
// multiple side pots

#ifndef Pot_h
#define Pot_h

#include<Seats.h>

namespace ds {

// type definitions

// PushedMoney represents money in front of a player, across the commit line
// This is then searched for duplicate all-in amounts and reduced to Pot type

struct PushedMoney:
    public std::pair<Money, SeatedPlayer>
{
    bool operator<(const PushMoney& pm) const {
        return first < pm.first;
    }
};

struct VecPushedMoney:
    public std::vector<PushedMoney>
{
    std::vector<PushedMoney>::iterator find(const SeatedPlayer& player) {
        for (auto it = begin(); it != end(); ++it) {
            if (it->second == player) {
                return it;
            }
        }
        return end();
    }
};

// Money is the amount in this pot, VecSeatedPlayer are all the players vying
// for this pot. Duplicates occur when two players are all in with the same
// amount, and the main pot for players who are not all in
struct Pot:
    public std::pair<Money, VecSeatedPlayer>
{
    bool operator<(const Pot& pt) const {
        return first < pt.first;
    }
};

struct VecPot:
    public std::vector<Pot>
{
    void collect(VecPushedMoney& pm) {
        std::sort(pm.begin(), pm.end());
        auto itPm = pm.begin();
        Money sum = 0;
        VecSeatedPlayer playersInPot;
        while (itPm != pm.end()) {
            sum += itPm->first;
            if (itPm->second == nullptr) {
                ++itPm;
            } else {
                playersInPot.push_back(itPm->second);
                break;
            }
        }
        for (
            auto itPmNxt = itPm + 1;
            itPmNext != pm.end();
            ++itPmNext
        ) {
            if (itPm->first == itPmNxt->first || itPmNxt->second == nullptr) {
                sum += itPmNxt->first;
                if (itPmNxt->second != nullptr) {
                    playersInPot.push_back(itPmNxt->second);
                }
            } else {
                // A(nother) player is all-in
                
                // Remove this amount from remaining pushes
                Money remove = itPm->first;
                for (auto itR = itPmNxt; itR != pm.end(); ++itR) {
                    sum += remove;
                    itR->first -= remove;
                }
                // Then search *this to find playersInPot
                if (size() && playersInPot.size()) {
                    VecSeatedPlayer& lastPlayerList(back().second());
                    // If one player is found, replace its player list with
                    // players in pot, and add sum to pot
                    bool found = (
                        std::find(
                            lastPlayerList.begin(),
                            lastPlayerList.end(),
                            playersInPot.back()
                        ) != lastPlayerList.end()
                    );
                    if (found) {
                        back().first += sum;
                        back().second = playersInPot;
                        sum = 0;
                        playersInPot.clear();
                        continue;
                    }
                }
                // If not found, create a new pot
                push_back(sum, playersInPot);
                sum = 0;
                playersInPot.clear();
            }
        }
        pm.clear();
    }

};

} // end namespace ds

#endif
