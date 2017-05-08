#include<algorithm>
#include<fstream>
#include<numeric>
#include<sstream>
#include<string>
#include<types.h>
#include<dsConfig.h>
#include<Card.h>
#include<Deck.h>
#include<DeckMask.h>
#include<HandRanker.h>
#include<PktCards.h>
#include<error.h>

using namespace ds;

int main() {
    size_t nTables;
    size_t nSeats;
    int nTableIters;
    int nResets;
    
    // Read inputSettings
    {
        ifstream is("inputSettings");
        is >> nTables;
        is >> nSeats;
        is >> nTableIters;
        is >> nResets;
    }
    Blinds blinds(0, 0, 1, 0);

    // Create tables
    std::vector<Table> tables(nTables, Table(nSeats, blinds, false, -1));

    // Create players
    size_t nPlayers = nSeats*nTables;
    std::vector<AllInPlayer> players(nPlayers);
    for (auto it = players.begin(); it != players.end(); ++it) {
        const size_t id = std::distance(it - players.begin());
        std::string name("player" + std::to_string(id));
        it->setIdAndName(id, name);
    }

    for (auto itT = tables.begin(); itT != tables.end(); ++itT) {
        auto itP = players.begin();
        size_t emptyChairs = nSeats;
        while (emptyChairs > 0) {
            it->addPlayer(&*itP++);
            emptyChairs--;
        }
        itT->setPlayerChips(Money(nTableIters*10));
        const size_t id = std::distance(itT - tables.begin());
        itT->setTableId(id);
    }

    std::thread tableThread[nTables];
    for (size_t ti = 0; ti < nTables; ++ti) {
        tableThread[ti] = std::thread(tables[ti].playNThenPause, 1000000);
    }

    while (nResets--) {
        for (size_t ti = 0; ti < nTables; ++ti) {
            tableThread[ti].join();
        }
        for (size_t ti = 0; ti < nTables; ++ti) {
            tables[ti].setPlayerChips(Money(nTableIters*10));
            tableThread[ti] = std::thread(tables[ti].playNThenPause, 1000000);
        }
    }
    for (size_t ti = 0; ti < nTables; ++ti) {
        tableThread[ti].join();
    }
    return 0;
}
