#include<algorithm>
#include<DeckMask.h>

// ****** Public Member Functions ****** //

void ds::HandRanker::predictFlop
(
    std::vector<short>& pa
) {
    if (bd.size()) {
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
