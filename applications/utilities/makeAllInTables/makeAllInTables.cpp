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
    Blinds blinds(0, 0, 1, 0);

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
        std::vector<std::thread> tableThread;
        tableThread.reserve(nTables);
        for (auto itT = tables.begin(); itT != tables.end(); ++itT) {
            tableThread.push_back(
                std::thread(startThread, std::ref(*itT), 1000000)
            );
        }

        for (size_t ti = 0; ti < nTables; ++ti) {
            tableThread[ti].join();
        }
    }

    //- Initialise output data
    std::vector<unsigned long long> nWon(323,0);
    std::vector<unsigned long long> nLost(323,0);
    std::vector<unsigned long long> winningFlopRankSum(323,0);
    std::vector<unsigned long long> winningTurnRankSum(323,0);
    std::vector<unsigned long long> winningRankSum(323,0);
    std::vector<unsigned long long> losingFlopRankSum(323,0);
    std::vector<unsigned long long> losingTurnRankSum(323,0);
    std::vector<unsigned long long> losingRankSum(323,0);

    for (auto it = players.begin(); it != players.end(); ++it) {
        const std::vector<unsigned long long>& pnWon(it->nWon());
        const std::vector<unsigned long long>& pnLost(it->nLost());
        const std::vector<unsigned long long>&
            pwinningFlopRankSum(it->winningFlopRankSum());
        const std::vector<unsigned long long>&
            pwinningTurnRankSum(it->winningTurnRankSum());
        const std::vector<unsigned long long>&
            pwinningRankSum(it->winningRankSum());
        const std::vector<unsigned long long>&
            plosingFlopRankSum(it->losingFlopRankSum());
        const std::vector<unsigned long long>&
            plosingTurnRankSum(it->losingTurnRankSum());
        const std::vector<unsigned long long>&
            plosingRankSum(it->losingRankSum());
        for (size_t i = 0; i < 323; ++i) {
            nWon[i] += pnWon[i];
            nLost[i] += pnLost[i];
            winningFlopRankSum[i] += pwinningFlopRankSum[i];
            winningTurnRankSum[i] += pwinningTurnRankSum[i];
            winningRankSum[i] += pwinningRankSum[i];
            losingFlopRankSum[i] += plosingFlopRankSum[i];
            losingTurnRankSum[i] += plosingTurnRankSum[i];
            losingRankSum[i] += plosingRankSum[i];
        }
    }
    
    // Write out results
    std::ofstream os("outputTables");
    os << "nWon nLost winningFlopRankSum winningTurnRankSum winningRankSum "
        << "losingFlopRankSum losingTurnRankSum losingRankSum\n";
    for (size_t i = 0; i < 323; ++i) {
        os << nWon[i] << " " << nLost[i] << " "
            << winningFlopRankSum[i] << " "
            << winningTurnRankSum[i] << " "
            << winningRankSum[i] << " "
            << losingFlopRankSum[i] << " "
            << losingTurnRankSum[i] << " "
            << losingRankSum[i] << std::endl;
    }
    
    return 0;
}
