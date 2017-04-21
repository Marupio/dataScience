#include<algorithm>
#include<DeckMask.h>

// ****** Public Member Functions ****** //

void ds::HandRanker::predict
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


// ****** END ****** //
