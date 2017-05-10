#include<Pot.h>
#include<algorithm>

std::ostream& ds::operator<<(std::ostream& os, const PushedMoney& pm) {
    os << "<" << pm.first << " ";
    if (*pm.second == nullptr) {
        os << "nullptr>";
    } else {
        os << (*pm.second)->name() << ">";
    }
    return os;
}


std::vector<ds::PushedMoney>::iterator ds::VecPushedMoney::find(
    const SeatedPlayer& player
) {
    for (auto it = begin(); it != end(); ++it) {
        if (it->second == player) {
            return it;
        }
    }
    return end();
}


std::ostream& ds::operator<<(std::ostream& os, const Pot& pt) {
    os << "<" << pt.first << " (";
    const VecSeatedPlayer& vsp(pt.second);
    auto itp = vsp.begin();
    if (itp != vsp.end()) {
        if (**itp == nullptr) {
            os << "nullptr";
        } else {
            os << (**itp)->name();
        }
        ++itp;
    }
    while (itp != vsp.end()) {
        if (**itp == nullptr) {
            os << " nullptr";
        } else {
            os << " " << (**itp)->name();
        }
        ++itp;
    }
    os << ")>";
    return os;
}


void ds::VecPot::collect(VecPushedMoney& pm) {
    if (!pm.size()) {
        return;
    }
    if (pm.size() == 1) {
        push_back(Pot(pm.front().first, VecSeatedPlayer(1, pm.front().second)));
        return;
    }

    std::sort(pm.begin(), pm.end());
    if (!size() || back().sealed) {
        //- Append a null pot to work with
        push_back(Pot());
    }
    Pot* workingPotPtr(&back());

    // Filter list of pushed players
    VecSeatedPlayer playersInPot;
    for (auto itPm = pm.begin(); itPm != pm.end(); ++itPm) {
        if ((*itPm->second)->hasCards()) {
            playersInPot.push_back(itPm->second);
        }
    }
    workingPotPtr->second = playersInPot;

    //- Keep track of all-in players not able to collect from side pots
    VecSeatedPlayer playersRemovedFromPot;

    auto itPm = pm.begin();
    workingPotPtr->first += itPm->first;

    // To account for edge-case where a player pushes the exact amount,
    // but is all-in
    bool allInEncountered = false;
    for (
        auto itPmNxt = itPm + 1;
        itPmNxt != pm.end();
        ++itPm, ++itPmNxt
    ) {
        workingPotPtr->first += itPmNxt->first;
        if (itPm->first + SMALL < itPmNxt->first) {
            // Amounts are different - a player folded or is all-in
            if (*itPm->second != nullptr && (*itPm->second)->hasCards()) {
                // Player is all-in
                // * Take equal contributions from remaining pushed money
                // * Seal pot
                // * Create a new pot
                Money allInAmt = itPm->first;
                for (auto itPmA = itPmNxt; itPmA != pm.end(); ++itPmA) {
                    workingPotPtr->first += allInAmt;
                    itPmA->first -= allInAmt;
                }
                workingPotPtr->sealed = true;
                
                // Remove player from other pots
                playersRemovedFromPot.push_back(itPm->second);
                for (
                    auto itPrp = playersRemovedFromPot.begin();
                    itPrp != playersRemovedFromPot.end();
                    ++itPrp
                ) {
                    for (
                        auto itPip = playersInPot.begin();
                        itPip != playersInPot.end();
                        ++itPip
                    ) {
                        if (*itPip == *itPrp) {
                            playersInPot.erase(itPip);
                            break;
                        }
                    }
                }
                
                push_back(Pot());
                workingPotPtr = &back();
                workingPotPtr->second = playersInPot;
                allInEncountered = false;
            }
        } else {
            // Amounts are the same
            if ((*itPmNxt->second)->allIn()) {
                allInEncountered = true;
                playersRemovedFromPot.push_back(itPmNxt->second);
            }
        }
    }
    if (allInEncountered) {
        workingPotPtr->sealed = true;
    }
    pm.clear();
}
