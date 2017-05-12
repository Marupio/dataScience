#include<iterator>
#include<fstream>
#include<functional>
#include<list>
#include<string>
#include<thread>
#include<types.h>
#include<dsConfig.h>
#include<Table.h>
#include<AllCallPlayer.h>

using namespace ds;

void startThread(Table& tbl, int n) {
    tbl.playNThenPause(n);
}


int main() {
    size_t nTables;
    size_t nSeats;
    int nTableIters;
    int nResets;
    bool tableLogging;
    
    // Read inputSettings
    {
        std::ifstream is("inputSettings");
        is >> nTables;
        is >> nSeats;
        is >> nTableIters;
        is >> nResets;
        is >> tableLogging;
    }
    Blinds blinds(0, 0.5, 1, 0);

    // Create players
    size_t nPlayers = nSeats*nTables;
    std::vector<AllCallPlayer> players(nPlayers);
    for (auto it = players.begin(); it != players.end(); ++it) {
        const size_t id = std::distance(players.begin(), it);
        std::string name("player" + std::to_string(id));
        it->setIdAndName(id, name);
    }

    // Create tables
    std::list<Table> tables;
    
    // Create tables and seat players, set starting chips
    size_t tableI = 0;
    auto itP = players.begin();
    while (itP != players.end()) {
        tables.emplace_back(nSeats, blinds, false, -1);
        Table& tbl(tables.back());
        tbl.setTableId(tableI++);
        if (tableLogging) {
            tbl.enableDataLogging();
        }
        size_t emptyChairs = nSeats;
        while (emptyChairs-- && itP != players.end()) {
            PlayerPtr player = &(*itP);
            tbl.addPlayer(player);
            ++itP;
        }
        tbl.setPlayerChips(Money(nTableIters*10));
    }

    //- Play games
    while (nResets--) {
        std::cout << nResets << std::endl;
        std::vector<std::thread> tableThread;
        tableThread.reserve(nTables);
        for (auto itT = tables.begin(); itT != tables.end(); ++itT) {
            tableThread.push_back(
                std::thread(startThread, std::ref(*itT), nTableIters)
            );
        }

        for (size_t ti = 0; ti < nTables; ++ti) {
            tableThread[ti].join();
        }
    }

    //- Initialise output data
    std::vector<std::vector<short>> wfr(324, std::vector<short>(0));
    std::vector<std::vector<short>> wtr(324, std::vector<short>(0));
    std::vector<std::vector<short>> wrr(324, std::vector<short>(0));
    std::vector<std::vector<short>> lfr(324, std::vector<short>(0));
    std::vector<std::vector<short>> ltr(324, std::vector<short>(0));
    std::vector<std::vector<short>> lrr(324, std::vector<short>(0));

    std::cout << "Gathering output data..." << std::endl;

    for (auto it = players.begin(); it != players.end(); ++it) {
        const std::vector<std::vector<short>>& pwfr(it->winningFlopRank());
        const std::vector<std::vector<short>>& pwtr(it->winningTurnRank());
        const std::vector<std::vector<short>>& pwrr(it->winningRiverRank());
        const std::vector<std::vector<short>>& plfr(it->losingFlopRank());
        const std::vector<std::vector<short>>& pltr(it->losingTurnRank());
        const std::vector<std::vector<short>>& plrr(it->losingRiverRank());

        wfr.insert(std::end(wfr), std::begin(pwfr), std::end(pwfr));
        wtr.insert(std::end(wtr), std::begin(pwtr), std::end(pwtr));
        wrr.insert(std::end(wrr), std::begin(pwrr), std::end(pwrr));
        lfr.insert(std::end(lfr), std::begin(plfr), std::end(plfr));
        ltr.insert(std::end(ltr), std::begin(pltr), std::end(pltr));
        lrr.insert(std::end(lrr), std::begin(plrr), std::end(plrr));
    }

    std::cout << "Writing out data..." << std::endl;

    for (CardVal cvA = Card::ace; cvA > Card::lowAce; --cvA) {
        for (CardVal cvB = cvA; cvB > Card::lowAce; --cvB) {
            for (size_t suited = 0; suited < 169; suited += 168) {
                if (suited && cvA == cvB) {
                    continue;
                }
                size_t hashIndex = (cvB - 2)*13 + cvA - 2 + suited;
                std::stringstream ss;
                ss << Card::valueToWriteChar(cvA)
                    << Card::valueToWriteChar(cvB);
                if (cvA != cvB) {
                    if (suited) {
                        ss << "s";
                    } else {
                        ss << "u";
                    }
                }
                {
                    std::ofstream os(ss.str() + "_wfr");
                    std::sort(wfr[hashIndex].begin(), wfr[hashIndex].end());
                    os << wfr[hashIndex];
                }
                {
                    std::ofstream os(ss.str() + "_wtr");
                    std::sort(wtr[hashIndex].begin(), wtr[hashIndex].end());
                    os << wtr[hashIndex];
                }
                {
                    std::ofstream os(ss.str() + "_wrr");
                    std::sort(wrr[hashIndex].begin(), wrr[hashIndex].end());
                    os << wrr[hashIndex];
                }
                {
                    std::ofstream os(ss.str() + "_lfr");
                    std::sort(lfr[hashIndex].begin(), lfr[hashIndex].end());
                    os << lfr[hashIndex];
                }
                {
                    std::ofstream os(ss.str() + "_ltr");
                    std::sort(ltr[hashIndex].begin(), ltr[hashIndex].end());
                    os << ltr[hashIndex];
                }
                {
                    std::ofstream os(ss.str() + "_lrr");
                    std::sort(lrr[hashIndex].begin(), lrr[hashIndex].end());
                    os << lrr[hashIndex];
                }
            }
        }
    }

    std::cout << "\nDone.\n" << std::endl;
    
    return 0;
}
