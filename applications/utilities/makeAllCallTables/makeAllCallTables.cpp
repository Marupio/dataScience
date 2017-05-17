#include <iterator>
#include <fstream>
#include <functional>
#include <list>
#include <sstream>
#include <string>
#include <thread>

#include <pqxx/pqxx>

#include <EntropyInterface.h>

#include <types.h>
#include <dsConfig.h>
#include <Table.h>
#include <AllCallPlayer.h>

using namespace ds;

void startThread(Table& tbl, int n) {
    tbl.playNThenPause(n);
}


int main() {
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
        
        C.disconnect ();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    size_t nTables;
    size_t nSeats;
    int nTableIters;
    int nResets;
    bool tableLogging;
    bool overwrite;
    // Read inputSettings
    {
        std::ifstream is("inputSettings");
        is >> nTables;
        is >> nSeats;
        is >> nTableIters;
        is >> nResets;
        is >> tableLogging;
        is >> overwrite;
    }
    EntropyInterface entropy;

    std::cout << "Beginning simulation:"
        << "        nTables = " << nTables
        << "\n         nSeats = " << nSeats
        << "\n    nTableIters = " << nTableIters
        << "\n         nResets = " << nResets
        << "\n    tableLogging = " << tableLogging
        << "\n       overwrite = " << overwrite << "\n" << std::endl;

    Blinds blinds(0, 0.5, 1, 0);
    std::string schemaName("pushfold");
    std::string tableName("allcall_" + std::to_string(nSeats));


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
        tables.emplace_back(entropy, nSeats, blinds, false, -1);
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
        Table& tbl(tables.front());
        tbl.playNThenPause(nTableIters);
    }

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

        {
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

        for (auto it = players.begin(); it != players.end(); ++it) {
            // Set reference for full data sets
            const std::vector<std::vector<short>>& wfr(it->winningFlopRank());
            const std::vector<std::vector<short>>& wtr(it->winningTurnRank());
            const std::vector<std::vector<short>>& wrr(it->winningRiverRank());
            const std::vector<std::vector<short>>& lfr(it->losingFlopRank());
            const std::vector<std::vector<short>>& ltr(it->losingTurnRank());
            const std::vector<std::vector<short>>& lrr(it->losingRiverRank());
            const std::vector<std::vector<std::vector<short>>>&
                wfp(it->winningFlopPredict());
            const std::vector<std::vector<std::vector<short>>>&
                wtp(it->winningTurnPredict());
            const std::vector<std::vector<std::vector<short>>>&
                lfp(it->losingFlopPredict());
            const std::vector<std::vector<std::vector<short>>>&
                ltp(it->losingTurnPredict());

            for (CardVal cvA = Card::ace; cvA > Card::lowAce; --cvA) {
                for (CardVal cvB = cvA; cvB > Card::lowAce; --cvB) {
                    for (size_t suited = 0; suited < 169; suited += 168) {
                        if (suited && cvA == cvB) {
                            continue;
                        }
                        size_t hashIndex = (cvB - 2)*13 + cvA - 2 + suited;
                        std::stringstream hand;
                        hand << Card::valueToWriteChar(cvA)
                            << Card::valueToWriteChar(cvB);
                        if (cvA != cvB) {
                            if (suited) {
                                hand << "s";
                            } else {
                                hand << "u";
                            }
                        }
                        for (size_t i = 0; i < wfr[hashIndex].size(); ++i) {
                            // Set references for individual game instance
                            char won;
                            short fr;
                            short tr;
                            short rr;
                            const std::vector<short>* fp;
                            const std::vector<short>* tp;
                            if (wfp[hashIndex].size()) {
                                // Won
                                won = 't';
                                fr = wfr[hashIndex][i];
                                tr = wtr[hashIndex][i];
                                rr = wrr[hashIndex][i];
                                fp = &wfp[hashIndex][i];
                                tp = &wtp[hashIndex][i];
                            } else {
                                // Lost
                                won = 'f';
                                fr = lfr[hashIndex][i];
                                tr = ltr[hashIndex][i];
                                rr = lrr[hashIndex][i];
                                fp = &lfp[hashIndex][i];
                                tp = &ltp[hashIndex][i];
                            }
                            #ifdef DSDEBUG
                            if (
                                fp->size() != 47
                             || tp->size() != 46
                            ) {
                                FatalError << "flop_predict or "
                                    << "turn_preduct incorrect size."
                                    << std::endl;
                                abort();
                            }
                            #endif

                            std::stringstream sql;
                            sql << "INSERT INTO " << schemaName << "."
                                << tableName
                                << "(hand,won,flop_rank,"
                                << "turn_rank,river_rank,flop_predict,"
                                << "turn_predict) VALUES ('"
                                << hand.str() << "','" << won << "'," << fr
                                << "," << tr << "," << rr << ",'{";
                            auto itf = fp->cbegin();
                            sql << *itf;
                            for (++itf; itf != fp->cend(); ++itf) {
                                sql << "," << *itf;
                            }
                            sql << "}','{";
                            auto itt = tp->cbegin();
                            sql << *itt;
                            for (++itt; itt != tp->cend(); ++itt) {
                                sql << "," << *itt;
                            }
                            sql << "}');";
                            pqxx::work W(C);
                            W.exec(sql.str().c_str());
                            W.commit();
                        } // instance
                    } // suited
                } // cvB
            } // cvA
        } // players
        
        C.disconnect ();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::cout << "\nDone.\n" << std::endl;
    
    return 0;
}