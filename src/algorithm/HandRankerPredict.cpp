#include <algorithm>
#include <DeckMask.h>

// ****** Public Member Functions ****** //

std::vector<short> ds::HandRanker::predictPreFlopToRiver() {
    if (bd_.size()) {
        FatalError << "Board must be clear to use this function." << std::endl;
        abort();
    }
    std::vector<short> pa;
    pa.reserve(108100);
    DeckMask dm(bd_, pkt_);
    for (auto itA = dm.cbegin(); itA != dm.cend(); ++itA) {
        for (auto itB = itA + 1; itB != dm.cend(); ++itB) {
            for (auto itC = itB + 1; itC != dm.cend(); ++itC) {
                for (auto itD = itC + 1; itD != dm.cend(); ++itD) {
                    for (auto itE = itD + 1; itE != dm.cend(); ++itE) {
                        cbd_ = bd_;
                        cbd_.flop(*itA, *itB, *itC);
                        cbd_.turn(*itD);
                        cbd_.river(*itE);
                        mask_ = baseMask_;
                        mask_.remove(cbd_);
                        pa.push_back(rank());
                    }
                }
            }
        }
    }
    return pa;
}


std::vector<short> ds::HandRanker::predictFlopToRiver() {
    if (bd_.size() != 3) {
        FatalError << "Board must be at flop to use this function." << std::endl;
        abort();
    }
    std::vector<short> pa;
    pa.reserve(2162);
    DeckMask dm(bd_, pkt_);
    size_t paSize = dm.size()*(dm.size() - 1)*(dm.size() - 2);
    pa.reserve(paSize);
    for (auto itA = dm.cbegin(); itA != dm.cend(); ++itA) {
        for (auto itB = itA + 1; itB != dm.cend(); ++itB) {
            cbd_ = bd_;
            cbd_.turn(*itA);
            cbd_.river(*itB);
            mask_ = baseMask_;
            mask_.remove(Card(*itA));
            mask_.remove(Card(*itB));
            pa.push_back(rank());
        }
    }
    return pa;
}


std::vector<short> ds::HandRanker::predictTurnToRiver() {
    if (bd_.size() != 4) {
        FatalError << "Board must be at turn to use this function." << std::endl;
        abort();
    }
    DeckMask dm(bd_, pkt_);
    std::vector<short> pa;
    pa.reserve(dm.size());
    for (auto it = dm.cbegin(); it != dm.cend(); ++it) {
        cbd_ = bd_;
        cbd_.river(*it);
        mask_ = baseMask_;
        mask_.remove(Card(*it));
        pa.push_back(rank());
    }
    return pa;
}


void ds::HandRanker::predictAndSort
(
    std::vector<short>& pa
) {
    DeckMask dm(bd_, pkt_);
    pa.clear();
    pa.reserve(dm.size());
    for (auto it = dm.cbegin(); it != dm.cend(); ++it) {
        cbd_ = bd_;
        switch (cbd_.size()) {
        case 3: {
            cbd_.turn(*it);
            break;
        }
        case 4: {
            cbd_.river(*it);
            break;
        }
        default: {
            FatalError << "Attempting to predict from board with "
                << cbd_.size() << " cards.  Expecting 3 or 4.\n" << cbd_
                << std::endl;
            abort();
        } // end default
        } // end switch
        mask_ = baseMask_;
        mask_.remove(Card(*it));
        pa.push_back(rank());
    }
    std::sort(pa.begin(), pa.end());
}


void ds::HandRanker::predictFlopAndSort
(
    std::vector<short>& pa
) {
    if (bd_.size()) {
        FatalError << "Board must be clear to use this function." << std::endl;
        abort();
    }
    DeckMask dm(bd_, pkt_);
    pa.clear();
    size_t paSize = dm.size()*(dm.size() - 1)*(dm.size() - 2);
    pa.reserve(paSize);
    for (auto itA = dm.cbegin(); itA != dm.cend(); ++itA) {
        for (auto itB = itA + 1; itB != dm.cend(); ++itB) {
            for (auto itC = itB + 1; itC != dm.cend(); ++itC) {
                cbd_ = bd_;
                cbd_.flop(*itA, *itB, *itC);
                mask_ = baseMask_;
                mask_.remove(cbd_);
                pa.push_back(rank());
            }
        }
    }
    std::sort(pa.begin(), pa.end());
}


// ****** END ****** //
