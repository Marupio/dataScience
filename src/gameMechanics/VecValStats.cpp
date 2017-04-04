#include<VecValStats.h>
#include<Board.h>

// ****** Constructors ****** //

ds::VecValStats::VecValStats(const VecCard& cards) {
    makeMetaData(cards);
}


ds::VecValStats::VecValStats(const Board& bd) {
    makeMetaData(bd.cards());
}


ds::VecValStats::VecValStats(const Board& bd, const PktCards& pkt) {
    VecCard tmpVc;
    tmpVc.reserve(bd.size() + 2);
    tmpVc.insert
    (
        bd.cards().begin(),
        tmpVc.begin(),
        tmpVc.begin() + bd.size()
    );
    tmpVc.push_back(pkt.first);
    tmpVc.push_back(pkt.second);
    makeMetaData(tmpVc);
}


ds::VecValStats::VecValStats(const VecDeckInd& vdi) {
    VecCard tmpVc;
    tmpVc.reserve (vdi.size());
    for (
        VecDeckInd::const_iterator it = vdi.begin();
        it != vdi.end();
        ++it
    ) {
        tmpVc.push_back(Card(*it));
    }
    makeMetaData(tmpVc);
}


// ****** Private Functions ****** //

void ds::VecValStats::makeMetaData(const VecCard& vc) {
    VecCardVal values;
    values.reserve(4);
    std::vector<short> counts;
    counts.reserve(4);
    VecSuitCount suits;
    suits.reserve(4);
        
    VecCard allCards = vc;
    std::sort(vc.rbegin(); vc.rend());
    for (
        std::pair<VecCard::const_iterator, VecCard::const_iterator> itPair(
            allCards.begin(), allCards.begin() + 1);
        itPair.second != allCards.end();
        ++itPair.first, ++itPair.second
    ) {
        #ifdef DSDEBUG
        if (!itPair.first->real() || !itPair.second->real()) {
            FatalError << "Stats requires real cards. No unknown, wild, lowAce "
                << "or out-of-range components.  Failing cards is one of "
                << "these:\n    " << *itPair.first << " " << *itPair.second
                << std::endl;
            abort()
        }
        #endif
        if (itPair.first->value() != itPair.second->value()) {
            values.push_back(itPair.second->value());
            counts.push_back(1);
            SuitCount sc;
            sc.fill(0);
            sc[it.Pair.second->suit()] = 1;
            suits.push_back(sc);
        } else {
            counts.back()++;
            suits.back()[itPair.second->suit()] += 1;
        }
    }

    data_.reserve(values.size());

    typedef std::tuple<
        VecCardVal::const_iterator,
        std::vector<short>::const_iterator,
        VecSuitCount::const_iterator
        > itValsCountsSuits;
    for (
        itValsCountsSuits itTrp(
            values.begin(),
            counts.begin(),
            suits.begin()
        );
        itTrp.get<0> != values.end();
    ) {
        ++itTrp.get<0>;
        ++itTrp.get<1>;
        ++itTrp.get<2>;
        data_.push_back(
            std::forward_as_tuple(
                itTrp.get<0>++
                itTrp.get<1>++
                itTrp.get<2>++
            )
        );
    }
}


} // ****** End ****** //
