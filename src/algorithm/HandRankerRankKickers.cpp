// ****** Private Member Functions ****** //

template <typename Avoider>
void ds::HandRanker::rankOneKicker(
    short& rank,
    const VecCard& reqdPktFirstCards,
    CardVal bdKicker,
    Avoider avoid
) {
    const VecValStats& stats(bd_.stats());
    auto itA = stats.cbegin();
    for (CardVal kicker = Card::ace; kicker > bdKicker; --kicker) {
        if (itA != stats.cend() && itA->value() == kicker) {
            ++itA;
            continue;
        }
        if (avoid(kicker)) {
            continue;
        }
        short subRankOffset = 0;
        for (
            auto itCard = reqdPktFirstCards.cbegin();
            itCard != reqdPktFirstCards.cend();
            ++itCard
        ) {
            PktCards testPkt(
                *itCard,
                kicker, Card::wildSuit
            );
            if (testPkt == pkt_) {
                return;
            } else {
                subRankOffset += mask_.remove(testPkt);
            }
        }
        rank += subRankOffset;
    }
}


template <typename Avoider>
void ds::HandRanker::rankTwoKickers(
    short& rank,
    const PktVals& bdKickers,
    Avoider avoid
) {
    const VecValStats& stats(bd_.stats());
    auto itH = stats.cbegin();
    for (
        CardVal highKicker = Card::ace;
        highKicker > bdKickers.second;
        --highKicker
    ) {
        if (itH != stats.cend() && itH->value() == highKicker) {
            ++itH;
            continue;
        }
        if (avoid(highKicker)) {
            continue;
        }
        
        auto itL = itH;
        for (
            CardVal lowKicker = highKicker - 1;
            lowKicker > bdKickers.first;
            --lowKicker
        ) {
            if (
                itL != stats.cend()
             && itL->value() == lowKicker
            ) {
                ++itL;
                continue;
            }
            if (avoid(lowKicker)) {
                continue;
            }
            PktCards testPkt(
                highKicker, Card::wildSuit,
                lowKicker, Card::wildSuit
            );
            if (testPkt == pkt_) {
                return;
            } else {
                rank += mask_.remove(testPkt);
            }
        }
        // Low kicker too low, continue through
        short rankOffset = 0;
        for (
            CardVal lowKicker = bdKickers.first - 1;
            lowKicker > Card::lowAce;
            --lowKicker
        ) {
            if (
                itL != stats.cend()
             && itL->value() == lowKicker
            ) {
                ++itL;
                continue;
            }
            if (avoid(lowKicker)) {
                continue;
            }
            PktCards testPkt(
                highKicker, Card::wildSuit,
                lowKicker, Card::wildSuit
            );
            if (testPkt == pkt_) {
                return;
            } else {
                rankOffset += mask_.remove(testPkt);
            }
        }
        rank += rankOffset;
    }
    // High kicker could still beat bdKickers.first
    for (
        CardVal highKicker = bdKickers.second - 1;
        highKicker > bdKickers.first;
        --highKicker
    ) {
        if (itH != stats.cend() && itH->value() == highKicker) {
            ++itH;
            continue;
        }
        if (avoid(highKicker)) {
            continue;
        }
        // Low kicker doesn't matter
        short rankOffset = 0;
        auto itL = itH;
        for (
            CardVal lowKicker = highKicker - 1;
            lowKicker > Card::lowAce;
            --lowKicker
        ) {
            if (
                itL != stats.cend()
             && itL->value() == lowKicker
            ) {
                ++itL;
                continue;
            }
            if (avoid(lowKicker)) {
                continue;
            }
            PktCards testPkt(
                highKicker, Card::wildSuit,
                lowKicker, Card::wildSuit
            );
            if (testPkt == pkt_) {
                return;
            } else {
                rankOffset += mask_.remove(testPkt);
            }
        }
        rank += rankOffset;
    }
}


void ds::HandRanker::rankKickersThreeFlush(short& rank) {
    const Suit flushSuit = bd_.flushSuit();
    const VecCardVal& flushValues(bd_.flushValues());
    auto itH = flushValues.cbegin();
    for (
        CardVal highKicker = Card::ace;
        highKicker > Card::two;
        --highKicker
    ) {
        if (itH != flushValues.cend() && highKicker == *itH) {
            ++itH;
            continue;
        }
        auto itL = itH;
        for (
            CardVal lowKicker = highKicker - 1;
            lowKicker > Card::lowAce;
            --lowKicker
        ) {
            if (itL != flushValues.cend() && lowKicker == *itL) {
                ++itL;
                continue;
            }
            PktCards testPkt(
                highKicker,
                flushSuit,
                lowKicker,
                flushSuit
            );
            if (pkt_ == testPkt) {
                return;
            } else {
                rank += mask_.remove(testPkt);
            }
        }
    }
    FatalError << "Rank should have been returned for flush.\n" << bd_ << " "
        << pkt_ << std::endl;
    abort();
}


void ds::HandRanker::rankKickersFourFlush(short& rank) {
    const Suit flushSuit = bd_.flushSuit();
    const VecCardVal& flushValues(bd_.flushValues());

    CardVal lowest = bd_.lowestFlushValue();
    auto itH = flushValues.cbegin();
    for (
        CardVal highKicker = Card::ace;
        highKicker > Card::lowAce;
        --highKicker
    ) {
        if (itH != flushValues.cend() && highKicker == *itH) {
            ++itH;
            continue;
        }
        auto itL = itH;
        for (
            CardVal lowKicker = highKicker - 1;
            lowKicker > lowest;
            --lowKicker
        ) {
            if (itL != flushValues.cend() && lowKicker == *itL) {
                ++itL;
                continue;
            }
            PktCards testPkt(
                highKicker, flushSuit,
                lowKicker, flushSuit
            );
            if (testPkt == pkt_) {
                return;
            } else {
                rank += mask_.remove(testPkt);
            }
        }
        // Low kicker too low
        PktCards testPkt(
            highKicker, flushSuit,
            Card::wildCard
        );
        if (testPkt == pkt_) {
            return;
        } else {
            rank += mask_.remove(testPkt);
        }
    }
    FatalError << "Rank should have been returned for flush.\n" << bd_ << " "
        << pkt_ << std::endl;
    abort();
}

void ds::HandRanker::rankKickersFiveFlush(short& rank) {
    const Suit flushSuit = bd_.flushSuit();
    const VecCardVal& flushValues(bd_.flushValues());
    const PktVals lowVals(bd_.lowestTwoFlushValues());
    auto itH = flushValues.cbegin();
    for (
        CardVal highKicker = Card::ace;
        highKicker > lowVals.second;
        --highKicker
    ) {
        if (itH != flushValues.cend() && highKicker == *itH) {
            ++itH;
            continue;
        }
        auto itL = itH;
        for (
            CardVal lowKicker = highKicker - 1;
            lowKicker > lowVals.first;
            --lowKicker
        ) {
            if (itL != flushValues.cend() && lowKicker == *itL) {
                ++itL;
                continue;
            }
            PktCards testPkt(highKicker, flushSuit, lowKicker, flushSuit);
            if (pkt_ == testPkt) {
                return;
            } else {
                rank += mask_.remove(testPkt);
            }
        }
        // No low kicker
        PktCards testPkt(highKicker, flushSuit, Card::wildCard);
        if (pkt_ == testPkt) {
            return;
        } else {
            rank += mask_.remove(testPkt);
        }
    }
    // High kicker could still beat lowest value
    for (
        CardVal highKicker = lowVals.second - 1;
        highKicker > lowVals.first;
        --highKicker
    ) {
        if (itH != flushValues.cend() && highKicker == *itH) {
            ++itH;
            continue;
        }
        PktCards testPkt(highKicker, flushSuit, Card::wildCard);
        if (pkt_ == testPkt) {
            return;
        } else {
            rank += mask_.remove(testPkt);
        }
    }
}

// ****** END ****** //
