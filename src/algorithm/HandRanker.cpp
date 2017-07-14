#include <iterator>
#include <HandRanker.h>
#include <error.h>

// ****** Public Static Data ****** //

#include <HandRankerShrrHist.h>

const short ds::HandRanker::maxRiverRank_ = 1325;

const size_t ds::HandRanker::preFlopPredictSize_ = 2118760;

const size_t ds::HandRanker::flopPredictSize_ = 1081;

const size_t ds::HandRanker::turnPredictSize_ = 46;


// ****** Constructors ****** //

ds::HandRanker::HandRanker(
    const Board& bd,
    const PktCards& pkt,
    bool includePktInMask
):
    pkt_(pkt),
    bd_(bd),
    cbd_(bd),
    baseMask_(
        includePktInMask
      ? PktMask(bd, pkt)
      : PktMask(bd)
    ),
    mask_(baseMask_)
{}


// ****** Public Member Functions ****** //

const std::vector<int>& ds::HandRanker::shrrHist(const PktCards& pkt) {
    #ifdef DSDEBUG
        size_t shi = pkt.startingHandIndex();
        if (shi < 0 || shi > 300) {
            FatalError << "Bad starting hand index (" << shi << ") for " << pkt << ", expecting "
                << "0..300" << std::endl;
            abort();
        }
        const std::vector<int>& res = shrrHist_[shi];
        if (res.size() == 0) {
            FatalError << "Bad starting hand index (" << shi << ") for " << pkt << ", associated "
                << "element is zero-sized." << std::endl;
            abort();
        }
        return res;
    #endif
    return shrrHist_[pkt.startingHandIndex()];
}


ds::Scalar ds::HandRanker::preflopWinProbRaw(const PktCards& pkt, size_t nOpponents) {
    const std::vector<int>& shrrHistI = shrrHist(pkt);
    Scalar prob = 0;
    short riverRank = 0;
    for (auto it = shrrHistI.cbegin(); it != shrrHistI.cend(); ++it) {
        if (!*it) {
            ++riverRank;
            continue;
        }

        prob += *it*riverWinProbRaw(riverRank, nOpponents);
        ++riverRank;
    }
    return prob/preFlopPredictSize_;
}


ds::Scalar ds::HandRanker::preflopWinProbRaw(size_t nOpponents) {
    return preflopWinProbRaw(pkt_, nOpponents);
}


ds::Scalar ds::HandRanker::flopTurnWinProbRaw(
    const std::vector<short>& riverRankHistogram,
    size_t nOpponents
) {
    Scalar prob = 0;
    int count = 0;
    short riverRank = 0;
    for (auto it = riverRankHistogram.cbegin(); it != riverRankHistogram.cend(); ++it) {
        if (!*it) {
            ++riverRank;
            continue;
        }
        count += *it;
        prob += *it*riverWinProbRaw(riverRank, nOpponents);
        ++riverRank;
    }
    return prob/count;
}


ds::Scalar ds::HandRanker::riverWinProbRaw(short riverRank, size_t nOpponents) {
    Scalar res = 1.;
    Scalar prob = (maxRiverRank_ - riverRank)/Scalar(maxRiverRank_);
    while (nOpponents--) {
        res *= prob;
    }
    return res;
}


ds::Scalar ds::HandRanker::preflopWinProbEst(
    const PktCards& pkt,
    const std::vector<std::pair<short, short>>& opponentRankEstimates
) {
    const std::vector<int>& shrrHistI = shrrHist(pkt);
    Scalar prob = 0;
    short riverRank = 0;
    for (auto it = shrrHistI.cbegin(); it != shrrHistI.cend(); ++it) {
        if (!*it) {
            ++riverRank;
            continue;
        }
        prob += *it*riverWinProbEst(riverRank, opponentRankEstimates);
        ++riverRank;
    }
    return prob/preFlopPredictSize_;
}


ds::Scalar ds::HandRanker::preflopWinProbEst(
    const std::vector<std::pair<short, short>>& opponentRankEstimates
) {
    return preflopWinProbEst(pkt_, opponentRankEstimates);
}


ds::Scalar ds::HandRanker::riverWinProbEst(
    short riverRank,
    const std::vector<std::pair<short, short>>& opponentRankEstimates
) {
    Scalar res = 1.;
    for (auto it = opponentRankEstimates.cbegin(); it != opponentRankEstimates.cend(); ++it) {
        if (it->first > riverRank) {
            // 1.0
            continue;
        }
        if (it->second < riverRank) {
            return 0.;
        }
        res *= (it->second - riverRank)/Scalar(it->second - it->first);
    }
    return res;
}


#include <HandRankerGetKickers.cpp>
#include <HandRankerCompare.cpp>
#include <HandRankerPredict.cpp>
#include <HandRankerRank.cpp>
#include <HandRankerRankKickers.cpp>

// ****** END ****** //
