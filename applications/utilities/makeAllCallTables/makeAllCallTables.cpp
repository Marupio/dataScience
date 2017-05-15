#include<iterator>
#include<fstream>
#include<functional>
#include<list>
#include<sstream>
#include<string>
#include<thread>

#include<pqxx/pqxx>

#include<types.h>
#include<dsConfig.h>
#include<Table.h>
#include<AllCallPlayer.h>

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

    //- Create table
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
        
        {
            std::stringstream sql;
            sql << "CREATE TABLE dummytable ("
                << "id serial primary key, test bool not null"
                << ")";
                
            /* Create a transactional object. */
            pqxx::work W(C);

            /* Execute SQL query */
            W.exec(sql.str().c_str());
            W.commit();
            std::cout << "Table created successfully" << std::endl;
        }

        {
            std::stringstream sql;
            sql << "INSERT INTO dummytable ("
                << "'t'"
                << ")";
                
            /* Create a transactional object. */
            pqxx::work W(C);

            /* Execute SQL query */
            W.exec(sql.str().c_str());
            W.commit();
            std::cout << "Row inserted successfully" << std::endl;
        }

        std::string tableName("allcall_" + std::to_string(nSeats));
        {
            std::stringstream sql;
            sql << "CREATE TABLE " << tableName << " ("
                << "id              bigserial primary key, "
                << "hand            varchar(3) not null, "
                << "won             bool not null, "
                << "flop_rank       smallint not null, "
                << "turn_rank       smallint not null, "
                << "river_rank      smallint not null, "
                << "flop_predict    smallint[] not null, "
                << "turn_predict    smallint[] not null, "
                << ")";
            /* Create a transactional object. */
            pqxx::work W(C);

            /* Execute SQL query */
            W.exec(sql.str().c_str());
            W.commit();
            std::cout << "Table created successfully" << std::endl;
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
                        for (size_t i = 0; i < wfr.size(); ++i) {
                            // Set references for individual game instance
                            const std::vector<short>& iwfr(wfr[i]);
                            const std::vector<short>& iwtr(wtr[i]);
                            const std::vector<short>& iwrr(wrr[i]);
                            const std::vector<short>& ilfr(lfr[i]);
                            const std::vector<short>& iltr(ltr[i]);
                            const std::vector<short>& ilrr(lrr[i]);
                            const std::vector<std::vector<short>>&
                                iwfp(wfp[i]);
                            const std::vector<std::vector<short>>&
                                iwtp(wtp[i]);
                            const std::vector<std::vector<short>>&
                                ilfp(lfp[i]);
                            const std::vector<std::vector<short>>&
                                iltp(ltp[i]);
                            char won;
                            short fr;
                            short tr;
                            short rr;
                            const std::vector<short>* fp;
                            const std::vector<short>* tp;
                            if (wfp[hashIndex].size()) {
                                // Won
                                #ifdef DSDEBUG
                                if (
                                    iwfp[hashIndex].size() != 47
                                 || iwtp[hashIndex].size() != 46
                                ) {
                                    FatalError << "flop_predict or "
                                        << "turn_preduct incorrect size."
                                        << std::endl;
                                    abort();
                                }
                                #endif
                                won = 't';
                                fr = iwfr[hashIndex];
                                tr = iwtr[hashIndex];
                                rr = iwrr[hashIndex];
                                fp = &iwfp[hashIndex];
                                tp = &iwtp[hashIndex];
                            } else {
                                // Lost
                                #ifdef DSDEBUG
                                if (
                                    ilfp[hashIndex].size() != 47
                                 || iltp[hashIndex].size() != 46
                                ) {
                                    FatalError << "flop_predict or "
                                        << "turn_preduct incorrect size."
                                        << std::endl;
                                    abort();
                                }
                                #endif
                                won = 'f';
                                fr = ilfr[hashIndex];
                                tr = iltr[hashIndex];
                                rr = ilrr[hashIndex];
                                fp = &ilfp[hashIndex];
                                tp = &iltp[hashIndex];
                            }

                            std::stringstream sql;
                            sql << "INSERT INTO " << tableName
                                << "(hand won flop_rank "
                                << "turn_rank river_rank flop_predict "
                                << "turn_predict) VALUES ("
                                << won << "," << fr << "," << tr << ","
                                << rr << ",{";
                            auto itf = fp->cbegin();
                            sql << *itf;
                            for (++itf; itf != fp->cend(); ++itf) {
                                sql << "," << *itf;
                            }
                            sql << "},{";
                            auto itt = tp->cbegin();
                            sql << *itt;
                            for (++itt; itt != tp->cend(); ++itt) {
                                sql << "," << *itt;
                            }
                            sql << "});";
                            /* Create a transactional object. */
                            pqxx::work W(C);

                            /* Execute SQL query */
                            W.exec(sql.str().c_str());
                            W.commit();
                            std::cout << "Row inserted successfully" << std::endl;
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
