#include<algorithm>
#include<fstream>
#include<numeric>
#include<types.h>
#include<dsConfig.h>
#include<HandRanker.h>
#include<Deck.h>
#include<DeckMask.h>
#include<PktCards.h>
#include<error.h>

using namespace ds;

template<class InputIt, class OutputIt, class Type, class MeanType>
void makeHistogramFromSortedData(
    InputIt istart,
    InputIt iend,
    OutputIt ostart,
    size_t nBuckets,
    Type minValue,
    Type maxValue,
    MeanType& average
) {
    sum = 0;
    Type delta = (maxValue - minValue)/nBuckets;
    InputIt lastIt = istart;
    for (Type cur = minValue; cur <= maxValue; cur += delta) {
        InputIt it = std::lower_bound(istart, iend, cur);
        *ostart = std::distance(lastIt, it);
        lastIt = it;
        average += *(ostart++)*(*it);
    }
    average /= MeanType(std::distance(istart, iend));
}


template<class InputIt, class Type>
void calculateStatisticsGivenMean(
    InputIt istart,
    InputIt iend,
    Type mean,
    Type& stdev,
    Type& skewness
) {
    stdev = 0;
    skewness = 0;
    for (auto it = istart; it != iend; ++i) {
        float diff = *it - mean;
        stdev += diff*diff;
        skewness += diff*diff*diff;
    }
    skewness /= std::pow(stdev, 1.5);
    size_t n = std::distance(istart, iend);
    stdev /= n;
    stdev = std::sqrt(stdev);
    skewness *= std::sqrt(n);
}


void makePreFlopData(const PktCards& pkt, const std::string& filename) {
    Board bd;
    HandRanker hr(bd, pkt);
    std::vector<short> pa;
    hr.predictFlop(pa);
    {
        std::ofstream os(filename, std::ofstream::binary);
        os << pa << std::endl;
    }
    {
        float mean;
        std::vector<short> hist(900);
        makeHistogramFromSortedData(
            pa.cbegin(),
            pa.cend(),
            hist.begin(),
            900,
            0,
            900,
            mean
        );
        {
            std::ofstream os(filename + "_hist");
            os << hist << std::endl;
        }
        float stdev, skewness;
        calculateStatisticsGivenMean(
            pa.cbegin(),
            pa.cend(),
            mean,
            stdev,
            skewness
        );
        {
            std::ofstream os(filename + "_stats");
            os << mean << " " << stdev << " " << skewness;
        }
        std::vector<int> accumulations(180);
        accumulations[0] =
            std::accumulate(hist.cbegin(), hist.cbegin() + 5, 0);
        for (size_t i = 1; i < 180; ++i) {
            accumulations[i] = accumulations[i-1] +
                std::accumulate(
                    hist.cbegin() + 5*i,
                    hist.cbegin() + 5*(i+1),
                    0
                );
        }
        {
            std::ofstream os(filename + "_accum");
            os << accumulations;
        }
    }
}


int main() {
    for (CardVal cvA = Card::ace; cvA > Card::lowAce; --cvA) {
        for (CardVal cvB = cvA; cvB > Card::lowAce; --cvB) {
            // Unsuited first
            PktCards pkt(cvA, Card::clubs, cvB, Card::diamonds);
            std::string filename(
                Card::cardValToWriteChar(cvA) +
                Card::cardValToWriteChar(cvB) + 'u'
            );
            makePreFlopData(pkt, filename);
            if (cvA != cvB) {
                // Suited next
                PktCards pkt(cvA, Card::clubs, cvB, Card::clubs);
                std::string filename(
                    Card::cardValToWriteChar(cvA) +
                    Card::cardValToWriteChar(cvB) + 's'
                );
                makePreFlopData(pkt, filename);
            }
        }
    }
    return 0;
}
