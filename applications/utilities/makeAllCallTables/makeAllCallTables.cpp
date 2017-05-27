#include <iterator>
#include <fstream>
#include <functional>
#include <list>
#include <thread>

#include <pqxx/pqxx>

#include <EntropyInterface.h>

#include <dsConfig.h>
#include <Dictionary.h>
#include <types.h>
#include <Table.h>
#include <AllCallPlayer.h>

using namespace ds;

void startThread(Table& tbl, int n) {
    tbl.playNThenPause(n);
}


int main(int argc, char *argv[]) {

    std::string prefix;
    if (argc > 1) {
        prefix = argv[1];
    } else {
        prefix = "inputSettings";
    }

    // Read inputSettings
    Dictionary inputDict(prefix);
    std::string schemaName(inputDict.lookup<std::string>("schemaName"));
    size_t nTables(inputDict.lookup<size_t>("nTables"));
    size_t nSeats(inputDict.lookup<size_t>("nSeats"));
    int nTableIters(inputDict.lookup<size_t>("nTableIters"));
    int nResets(inputDict.lookup<size_t>("nResets"));
    bool tableLogging(inputDict.lookup<bool>("tableLogging"));
    bool overwrite(inputDict.lookup<bool>("overwrite"));
    bool multiThreading(inputDict.lookup<bool>("multiThreading"));

    std::string tableName("allcall_" + std::to_string(nSeats));

    try {
        pqxx::connection C("dbname = dsdata user = dsuser password = 123 \
        hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            std::cout << "Opened database successfully: " << C.dbname()
            << std::endl;
        } else {
            std::cout << "Can't open database" << std::endl;
            return 1;
        }
        
        //- Check if schema exists
        bool schemaExists = false;
        {
            std::stringstream sql;
            sql << "SELECT schema_name FROM information_schema.schemata "
                << "WHERE schema_name = '" << schemaName << "';";
            pqxx::nontransaction N(C);
            pqxx::result R(N.exec(sql));
            if (R.size()) {
                schemaExists = true;
            }
        }

        // Create schema if it doesn't exist
        if (!schemaExists) {
            std::stringstream sql;
            sql << "CREATE SCHEMA " << schemaName << ";";
            pqxx::work W(C);
            W.exec(sql.str().c_str());
            W.commit();
        }

        //- Check if table exists
        bool tableExists = false;
        {
            std::stringstream sql;
            sql << "SELECT c.oid, "
                << "  n.nspname, "
                << "  c.relname "
                << "FROM pg_catalog.pg_class c "
                << "     LEFT JOIN pg_catalog.pg_namespace n "
                << "        ON n.oid = c.relnamespace "
                << "WHERE c.relname ~ '^(" << tableName << ")$' "
                << "  AND n.nspname ~ '^(" << schemaName << ")$';";

            pqxx::nontransaction N(C);
            pqxx::result R(N.exec(sql));
            if (R.size()) {
                tableExists = true;
            }
        }

        // Drop table if it exists and overwriting is allowed
        if (tableExists && overwrite) {
            std::stringstream sql;
            sql << "DROP TABLE " << schemaName << "." << tableName << ";";
            pqxx::work W(C);
            W.exec(sql.str().c_str());
            W.commit();
        }

        if (!tableExists || overwrite) {
            std::stringstream sql;
            sql << "CREATE TABLE " << schemaName << "." << tableName << " ("
                << "id              bigserial primary key, "
                << "hand            varchar(3) not null, "
                << "won             bool not null, "
                << "flop_rank       smallint not null, "
                << "turn_rank       smallint not null, "
                << "river_rank      smallint not null, "
                << "flop_predict    smallint[] not null, "
                << "turn_predict    smallint[] not null "
                << ")";
            pqxx::work W(C);
            W.exec(sql.str().c_str());
            W.commit();
        }

        C.disconnect ();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    EntropyInterface entropy;
    Blinds blinds(0, 0.5, 1, 0);

    std::cout << "Beginning simulation:\n" << inputDict << std::endl;

    // Create players
    size_t nPlayers = nSeats*nTables;
    std::vector<AllCallPlayer> players(nPlayers);
    for (auto it = players.begin(); it != players.end(); ++it) {
        const size_t id = std::distance(players.begin(), it);
        std::string name("player" + std::to_string(id));
        it->initialise(id, name, schemaName, tableName);
    }

    // Create tables
    std::list<Table> tables;
    
    // Create tables and seat players, set starting chips
    size_t tableI = 0;
    auto itP = players.begin();
    while (itP != players.end()) {
        std::stringstream randomizer;
        randomizer << "makeAllCallTables_" << tableI;
        tables.emplace_back(entropy, nSeats, blinds, false, -1, randomizer.str());
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
        if (multiThreading) {
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
        } else {
            Table& tbl(tables.front());
            tbl.playNThenPause(nTableIters);
        }
    }
    std::cout << "\nDone.\n" << std::endl;
    
    return 0;
}