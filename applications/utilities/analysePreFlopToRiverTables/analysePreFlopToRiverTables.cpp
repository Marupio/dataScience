#include <fstream>

#include <dsConfig.h>
#include <types.h>
#include <Deck.h>
#include <DeckMask.h>
#include <HandRanker.h>
#include <PktCards.h>

std::string schemaName("staticdata");
std::string tableName("prefloptoriver");

using namespace ds;


int main(int argc, char *argv[]) {
    std::ofstream os("analysePreFlopToRiverTables_output");
    os << "hand 1 2 3 4 5 6 7 8 9 10 11\n";
    for (CardVal cvA = Card::ace; cvA > Card::lowAce; --cvA) {
        for (CardVal cvB = cvA; cvB > Card::lowAce; --cvB) {
            std::cout << "Working on " << Card::valueToWriteChar(cvA)
                << Card::valueToWriteChar(cvB) << std::endl;
            // Unsuited first
            {
                PktCards pkt(cvA, Card::clubs, cvB, Card::diamonds);
                os << pkt.startingHandName();
                for (size_t nOpp = 1; nOpp < 12; ++nOpp)
                {
                    os << " " << HandRanker::preflopWinProbRaw(pkt, nOpp);
                }
                os << std::endl;
            }
            if (cvA != cvB) {
                // Suited next
                PktCards pkt(cvA, Card::clubs, cvB, Card::clubs);
                os << pkt.startingHandName();
                for (size_t nOpp = 1; nOpp < 12; ++nOpp)
                {
                    os << " " << HandRanker::preflopWinProbRaw(pkt, nOpp);
                }
                os << std::endl;
            }
        }
    }
    return 0;
}


// ****** END ****** //