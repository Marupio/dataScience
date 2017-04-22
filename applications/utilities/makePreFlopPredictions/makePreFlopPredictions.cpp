#include<algorithm>
#include<fstream>
#include<numeric>
#include<sstream>
#include<string>
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
    average = 0;
    Type delta = (maxValue - minValue)/nBuckets;
    InputIt lastIt = istart;
    for (Type cur = minValue; cur < maxValue; cur += delta) {
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
    for (auto it = istart; it != iend; ++it) {
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


void makePreFlopData(
    const PktCards& pkt,
    const std::string& filename,
    std::ofstream& oss
) {
    Board bd;
    HandRanker hr(bd, pkt);
    std::vector<short> pa;
    hr.predictFlop(pa);
    {
        std::ofstream os(filename);//, std::ofstream::binary);
        for (auto it = pa.cbegin(); it != pa.cend(); ++it) {
            os << *it << "\n";
        }
        os << std::endl;
    }
    {
        float mean;
        std::vector<short> hist(90);
        makeHistogramFromSortedData(
            pa.cbegin(),
            pa.cend(),
            hist.begin(),
            90,
            0,
            900,
            mean
        );
        {
            std::ofstream os(filename + "_hist");
            for (auto it = hist.cbegin(); it != hist.cend(); ++it) {
                os << *it << "\n";
            }
            os << std::endl;
        }
        float stdev = 0;
        float skewness = 0;
        calculateStatisticsGivenMean(
            pa.cbegin(),
            pa.cend(),
            mean,
            stdev,
            skewness
        );
        oss << filename << " " << mean << " " << stdev << " " << skewness
            << std::endl;
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
            for (
                auto it = accumulations.cbegin();
                it != accumulations.cend();
                ++it
            ) {
                os << *it << "\n";
            }
            os << std::endl;
        }
    }
}


int main() {
    std::ofstream oss("summary");
    oss << "pkt mean stdev skewness" << std::endl;
    for (CardVal cvA = Card::ace; cvA > Card::lowAce; --cvA) {
        std::cout << "Working on " << Card::valueToWriteChar(cvA) << std::endl;
        for (CardVal cvB = cvA; cvB > Card::lowAce; --cvB) {
            // Unsuited first
            {
                PktCards pkt(cvA, Card::clubs, cvB, Card::diamonds);
                std::stringstream ss;
                std::string filename;
                ss << Card::valueToWriteChar(cvA) << Card::valueToWriteChar(cvB);
                ss << "u";
                ss >> filename;
                makePreFlopData(pkt, filename, oss);
            }
            if (cvA != cvB) {
                // Suited next
                PktCards pkt(cvA, Card::clubs, cvB, Card::clubs);
                std::stringstream ss;
                std::string filename;
                ss << Card::valueToWriteChar(cvA) << Card::valueToWriteChar(cvB);
                ss << "s";
                ss >> filename;
                makePreFlopData(pkt, filename, oss);
            }
        }
    }
    return 0;
}
