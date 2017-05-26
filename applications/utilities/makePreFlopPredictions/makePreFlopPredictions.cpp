#include <algorithm>
#include <fstream>
#include <numeric>
#include <types.h>
#include <dsConfig.h>
#include <Card.h>
#include <Deck.h>
#include <DeckMask.h>
#include <HandRanker.h>
#include <PktCards.h>
#include <error.h>

using namespace ds;

class Accumulator:
    public std::array<int, 900>
{
public:

    std::string match;

    bool operator<(const Accumulator& a) {
        for (
            std::pair<
                Accumulator::const_reverse_iterator,
                Accumulator::const_reverse_iterator
            > itPair(this->crbegin(), a.crbegin());
            itPair.first != this->crend();
            ++itPair.first, ++itPair.second
        ) {
            if (*itPair.first == *itPair.second) {
                continue;
            }
            return (*itPair.first > *itPair.second);
        }
        return false;
    }
    
};


typedef std::array<Accumulator, 169> ArrAccumulator;


void writeArrAccumulator(std::ostream& os, const ArrAccumulator& a) {
    for (auto it = a.cbegin(); it != a.cend(); ++it) {
        os << it->match << " ";
        for (auto itR = it->cbegin(); itR != it->cend(); ++itR) {
            os << *itR << " ";
        }
        os << std::endl;
    }
}

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
        InputIt it = std::lower_bound(istart, iend, cur+1);
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
    size_t index,
    ArrAccumulator& acc,
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
        Accumulator& accI = acc[index];
        accI[0] = hist.front();
        for (size_t i = 1; i < hist.size(); ++i) {
            accI[i] = accI[i-1] + hist[i];
        }
        oss << mean << " " << stdev << " " << skewness
            << std::endl;
        {
            std::ofstream os(filename + "_accum");
            for (
                auto it = accI.cbegin();
                it != accI.cend();
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
    oss << "pkt chen mean stdev skewness" << std::endl;
    ArrAccumulator acc;
    size_t index = 0;
    for (CardVal cvA = Card::ace; cvA > Card::lowAce; --cvA) {
        std::cout << "Working on " << Card::valueToWriteChar(cvA) << std::endl;
        for (CardVal cvB = cvA; cvB > Card::lowAce; --cvB) {
            int chen = 0;
            if (cvA <= 10) {
                chen = cvA/2;
            } else if (cvA == Card::ace) {
                chen = 10;
            } else if (cvA == Card::king) {
                chen = 8;
            } else if (cvA == Card::queen) {
                chen = 7;
            } else if (cvA == Card::jack) {
                chen = 6;
            }
            short delta = cvA - cvB;
            switch (delta) {
            case 0:
                chen *= 2;
                // fall through
            case 1:
                if (cvA < Card::king) {
                    chen += 1;
                }
                break;
            case 2:
                chen -= 1;
                break;
            case 3:
                chen -= 2;
                break;
            case 4:
                chen -= 4;
                break;
            default:
                chen -= 5;
                break;
            } // end switch
            
            // Unsuited first
            {
                PktCards pkt(cvA, Card::clubs, cvB, Card::diamonds);
                std::stringstream ss;
                std::string filename;
                ss << Card::valueToWriteChar(cvA) << Card::valueToWriteChar(cvB);
                if (cvA != cvB) {
                    ss << "u";
                }
                ss >> filename;
                acc[index].match = filename;
                oss << filename << " " << chen << " ";
                makePreFlopData(pkt, filename, index, acc, oss);
                index++;
            }
            if (cvA != cvB) {
                // Suited next
                chen += 2;
                PktCards pkt(cvA, Card::clubs, cvB, Card::clubs);
                std::stringstream ss;
                std::string filename;
                ss << Card::valueToWriteChar(cvA) << Card::valueToWriteChar(cvB);
                ss << "s";
                ss >> filename;
                acc[index].match = filename;
                oss << filename << " " << chen << " ";
                makePreFlopData(pkt, filename, index, acc, oss);
                index++;
            }
        }
        std::sort(acc.begin(), acc.end());
        std::ofstream os("accumulations");
        writeArrAccumulator(os, acc);
    }
    return 0;
}
