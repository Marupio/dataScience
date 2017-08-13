#include <pqxxInterface.h>

#include <types.h>
#include <Board.h>

int main(int argc, char const *argv[])
{

    // Prepare postgres data tables
    // schema: edf
    // table: flopHashes: hash (int), board (string)
    // table: turnHashes: hash (int), board (string)
    // table: riverHashes: hash (int), board (string)
    // table: flopAccum: hash (int), accum (bigint[])
    // table: turnAccum: hash (int), accum (bigint[])
    // table: riverAccum: hash (int), accum (bigint[])
    bool overwrite = false;
    if (argc > 1) {
        if (argv[1] == "-o" || argv[1] == "--overwrite") {
            overwrite = true;
        } else {
            std::cout << "Unrecognized command-line arguments: " << argv[1] << ".\n";
            std::cout << "\nUsage:\n    makeAllEmpiricalDistributionFunctions [-o|--overwrite]\n";
            std::cout << "   -o, --overwrite  If database tables already exist, overwrite them\n";
            std::cout << std::endl;
            abort();
        }
    }
    {
        pqxxInterface db(
            "dbname = dsdata user = dsuser password = 123 hostaddr = 127.0.0.1 port = 5432"
        );

        std::string schemaName("edf");

        if (!db.foundSchema(schemaName)) {
            db.createSchema(schemaName);
        }

        // *** Valid hash tables *** //

        std::string tableName;
        std::vector<std::pair<std::string, std::string>> hashHeadingsAndTypes =
            {
                {"hash", "bigint not null"},
                {"board", "text"}
            };

        // table: flopHashes: hash (int), board (string)
        tableName = "flophashes";
        bool tableExists = db.foundSchemaTable(schemaName, tableName);
        if (tableExists && overwrite) {
            db.dropTable(schemaName, tableName);
        }
        if (!tableExists || overwrite) {
            db.createTable(schemaName, tableName, hashHeadingsAndTypes);
        }

        // table: turnHashes: hash (int), board (string)
        tableName = "turnhashes";
        tableExists = db.foundSchemaTable(schemaName, tableName);
        if (tableExists && overwrite) {
            db.dropTable(schemaName, tableName);
        }
        if (!tableExists || overwrite) {
            db.createTable(schemaName, tableName, hashHeadingsAndTypes);
        }

        // table: riverHashes: hash (int), board (string)
        tableName = "riverhashes";
        tableExists = db.foundSchemaTable(schemaName, tableName);
        if (tableExists && overwrite) {
            db.dropTable(schemaName, tableName);
        }
        if (!tableExists || overwrite) {
            db.createTable(schemaName, tableName, hashHeadingsAndTypes);
        }


        // *** Accumulation tables *** //

        std::vector<std::pair<std::string, std::string>> accumHeadingsAndTypes =
            {
                {"hash", "bigint not null"},
                {"accum", "bigint[] not null"}
            };

        // table: flopAccum: hash (int), accum (bigint[])
        tableName = "flopaccum";
        tableExists = db.foundSchemaTable(schemaName, tableName);
        if (tableExists && overwrite) {
            db.dropTable(schemaName, tableName);
        }
        if (!tableExists || overwrite) {
            db.createTable(schemaName, tableName, accumHeadingsAndTypes);
        }

        // table: turnAccum: hash (int), accum (bigint[])
        tableName = "turnaccum";
        tableExists = db.foundSchemaTable(schemaName, tableName);
        if (tableExists && overwrite) {
            db.dropTable(schemaName, tableName);
        }
        if (!tableExists || overwrite) {
            db.createTable(schemaName, tableName, accumHeadingsAndTypes);
        }

        // table: riverAccum: hash (int), accum (bigint[])
        tableName = "riveraccum";
        tableExists = db.foundSchemaTable(schemaName, tableName);
        if (tableExists && overwrite) {
            db.dropTable(schemaName, tableName);
        }
        if (!tableExists || overwrite) {
            db.createTable(schemaName, tableName, accumHeadingsAndTypes);
        }


        // *** Empirical distribution function tables *** //

        std::vector<std::pair<std::string, std::string>> edfHeadingsAndTypes =
            {
                {"hash", "bigint not null"},
                {"edf", "real[] not null"}
            };

        // table: flopedf: hash (int), edf (real[])
        tableName = "flopedf";
        tableExists = db.foundSchemaTable(schemaName, tableName);
        if (tableExists && overwrite) {
            db.dropTable(schemaName, tableName);
        }
        if (!tableExists || overwrite) {
            db.createTable(schemaName, tableName, edfmHeadingsAndTypes);
        }

        // table: turnedf: hash (int), edf (real[])
        tableName = "turnedf";
        tableExists = db.foundSchemaTable(schemaName, tableName);
        if (tableExists && overwrite) {
            db.dropTable(schemaName, tableName);
        }
        if (!tableExists || overwrite) {
            db.createTable(schemaName, tableName, edfmHeadingsAndTypes);
        }

        // table: riveredf: hash (int), edf (real[])
        tableName = "riveredf";
        tableExists = db.foundSchemaTable(schemaName, tableName);
        if (tableExists && overwrite) {
            db.dropTable(schemaName, tableName);
        }
        if (!tableExists || overwrite) {
            db.createTable(schemaName, tableName, edfmHeadingsAndTypes);
        }
    }

    std::vector<int> validFlopHashes;
    std::vector<int> validTurnHashes;
    std::vector<int> validRiverHashes;
    std::vector<std::vector<long long>> flopAccum(12500);
    std::vector<std::vector<long long>> turnAccum(5764801);
    std::vector<std::vector<long long>> riverAccum(254803968);
    for (DeckInd dA = 0; dA < 48; ++dA) {
        for (DeckInd dB = dA + 1; dB < 49; ++dB) {
            for (DeckInd dC = dB + 1; dC < 50; ++dC) {
                std::cout << dA << "/47 " << dB << "/48 " << dC << "/49 " << std::endl;
                Board bdFlop(dA, dB, dC);
                int flopHash = bdFlop.perfectFlopHash();
                validFlopHashes.push_back(flopHash);
                std::vector<long long>& flopAccumI = flopAccum[flopHash];
                flopAccumI = std::vector<long long>(1226, 0);
                for (DeckInd dD = dC + 1; dD < 51; ++dD) {
                    Board bdTurn(dA, dB, dC, dD);
                    int turnHash = bdTurn.perfectTurnHash();
                    validTurnHashes.push_back(turnHash);
                    std::vector<long long>& turnAccumI = turnAccum[turnHash];
                    turnAccumI = std::vector<long long>(1226, 0);
                    for (DeckInd dE = dD + 1; dE < 52; ++dE) {
                        Board bdRiver(dA, dB, dC, dD, dE);
                        int riverHash = bdTurn.perfectRiverHash();
                        validRiverHashes.push_back(riverHash);
                        std::vector<long long>& riverI(riverAccum[riverHash]);
                        riverI = std::vector<long long>(1226, 0);
                        for (DeckInd dPA = 0; dPA < 51; ++dPA) {
                            if (dPA == dA || dPA == dB || dPA == dC || dPA == dD || dPA == dE) {
                                continue;
                            }
                            for (DeckInd dPB = dPA + 1; dPB < 52; ++dPB) {
                                if (dPB == dA || dPB == dB || dPB == dC || dPB == dD || dPB == dE) {
                                    continue;
                                }
                                PktCards pkt(dPA, dPB);
                                HandRanker hr(bdRiver, pkt);
                                short rank = hr.rank();
                                ++riverAccumI[rank];
                            }
                        } // end pkts
                        // convert riverAccumI which is currently occurences to accumulative
                        // (12 16 10 5 8 ...) --> (12 28 38 43 51)
                        // i.e. n less than i
                        // add accumulative to turnCounts
                        auto it = riverI.begin();
                        long long accum = 0;
                        for (auto it = riverI.begin(); it != riverI.end(); ++it) {
                            accum += *it;
                            *it = accum;
                        }
                        // Now riverAccum[riverHash] is correct
                        // Add to turn and flop accum
                        auto itR = riverI.cbegin();
                        auto itF = flopI.begin();
                        auto ifT = turnI.begin();
                        for (; itR != riverI.cend(); ++itR) {
                            *itF += *itR;
                            *itT += *itR;
                            ++itF;
                            ++itT;
                        }
                    } // end river
                } // end turn
            } // flop C
        } // flop B
    } // flop A

    // Output raw data to postgres
    // std::vector<int> validFlopHashes;
    // std::vector<int> validTurnHashes;
    // std::vector<int> validRiverHashes;
    // std::vector<std::vector<long long>> flopAccum(12500);
    // std::vector<std::vector<long long>> turnAccum(5764801);
    // std::vector<std::vector<long long>> riverAccum(254803968);

    {
        pqxxInterface db(
            "dbname = dsdata user = dsuser password = 123 hostaddr = 127.0.0.1 port = 5432"
        );

        // table: flopHashes: hash (int), board (string)
        // table: flopAccum: hash (int), accum (bigint[])
        std::cout << "Writing raw flop data..." << std::endl;
        for (auto it = validFlopHashes.cbegin(); it != validFlopHashes.cend(); ++it) {
            {
                std::stringstream sql;
                Board bd(Board::makeBoardFromFlopHash(*it));
                sql << "INSERT INTO " << schemaName << ".flophashes(hash,board) VALUES (" << *it
                    << ",'" << bd << "');"
                db.work(sql);
            }
            {
                const std::vector<long long>& flopAccumI(flopAccum[*it]);
                std::stringstream sql;
                sql << "INSERT INTO " << schemaName << ".flopaccum(hash,accum) VALUES (" << *it
                    << ",'{";
                auto accumIt = flopAccumI.cbegin();
                sql << *accumIt;
                for (++accumIt; accumIt != flopAccumI.cend(); ++accumIt) {
                    sql << "," << *accumIt;
                }
                sql << "}');";
                db.work(sql);
            }
        }

        // table: turnHashes: hash (int), board (string)
        // table: turnAccum: hash (int), accum (bigint[])
        std::cout << "Writing raw turn data..." << std::endl;
        for (auto it = validTurnHashes.cbegin(); it != validTurnHashes.cend(); ++it) {
            {
                std::stringstream sql;
                Board bd(Board::makeBoardFromTurnHash(*it));
                sql << "INSERT INTO " << schemaName << ".turnhashes(hash,board) VALUES (" << *it
                    << ",'" << bd << "');"
                db.work(sql);
            }
            {
                const std::vector<long long>& turnAccumI(turnAccum[*it]);
                std::stringstream sql;
                sql << "INSERT INTO " << schemaName << ".turnaccum(hash,accum) VALUES (" << *it
                    << ",'{";
                auto accumIt = turnAccumI.cbegin();
                sql << *accumIt;
                for (++accumIt; accumIt != turnAccumI.cend(); ++accumIt) {
                    sql << "," << *accumIt;
                }
                sql << "}');";
                db.work(sql);
            }
        }

        // table: riverHashes: hash (int), board (string)
        // table: riverAccum: hash (int), accum (bigint[])
        std::cout << "Writing raw river data..." << std::endl;
        for (auto it = validRiverHashes.cbegin(); it != validRiverHashes.cend(); ++it) {
            {
                std::stringstream sql;
                Board bd(Board::makeBoardFromRiverHash(*it));
                sql << "INSERT INTO " << schemaName << ".riverhashes(hash,board) VALUES (" << *it
                    << ",'" << bd << "');"
                db.work(sql);
            }
            {
                const std::vector<long long>& riverAccumI(riverAccum[*it]);
                std::stringstream sql;
                sql << "INSERT INTO " << schemaName << ".riveraccum(hash,accum) VALUES (" << *it
                    << ",'{";
                auto accumIt = riverAccumI.cbegin();
                sql << *accumIt;
                for (++accumIt; accumIt != riverAccumI.cend(); ++accumIt) {
                    sql << "," << *accumIt;
                }
                sql << "}');";
                db.work(sql);
            }
        }
    }

    std::cout << "Parsing flop data..." << std::endl;
    {
        std::vector<std::vector<float>> edfFlop(validFlopHashes.size());
        auto itEdfOuter = edfFlop.begin();
        for (auto it = validFlopHashes.cbegin(); it != validFlopHashes.cend(); ++it) {
            const std::vector<long long>& flopAccumI(flopAccum[*it]);
            const float rTotal = 1.0/float(flopAccumI.back());
            itEdfOuter->reserve(flopAccumI.size());
            auto itEdfInner = itEdfOuter->begin();
            for (auto itAccum = flopAccumI.cbegin(); itAccum != flopAccumI.cend(); ++itAccum) {
                *itEdfInner = float(*itAccum)*rTotal;
                ++itEdfInner;
            }
            ++itEdfOuter;
        }
        std::cout << "Writing flop edf..." << std::endl;

        pqxxInterface db(
            "dbname = dsdata user = dsuser password = 123 hostaddr = 127.0.0.1 port = 5432"
        );

        std::cout << "Writing raw flop data..." << std::endl;
        auto itEdfOuter = edfFlop.cbegin();
        auto it = validFlopHashes.cbegin();
        for (; it != validFlopHashes.cend(); ++it, ++itEdfOuter) {
            const std::vector<long long>& flopAccumI(flopAccum[*it]);
            std::stringstream sql;
            sql << "INSERT INTO " << schemaName << ".flopedf(hash,edf) VALUES (" << *it
                << ",'{";
            auto itEdfInner = itEdfOuter->cbegin();
            sql << *itEdfInner;
            for (++itEdfInner; itEdfInner != itEdfOuter->cend(); ++itEdfInner) {
                sql << "," << *itEdfInner;
            }
            sql << "}');";
            db.work(sql);
        }
    }

    std::cout << "Parsing turn data..." << std::endl;
    {
        std::vector<std::vector<float>> edfTurn(validTurnHashes.size());
        auto itEdfOuter = edfTurn.begin();
        for (auto it = validTurnHashes.cbegin(); it != validTurnHashes.cend(); ++it) {
            const std::vector<long long>& turnAccumI(turnAccum[*it]);
            const float rTotal = 1.0/float(turnAccumI.back());
            itEdfOuter->reserve(turnAccumI.size());
            auto itEdfInner = itEdfOuter->begin();
            for (auto itAccum = turnAccumI.cbegin(); itAccum != turnAccumI.cend(); ++itAccum) {
                *itEdfInner = float(*itAccum)*rTotal;
                ++itEdfInner;
            }
            ++itEdfOuter;
        }
        std::cout << "Writing turn edf..." << std::endl;

        pqxxInterface db(
            "dbname = dsdata user = dsuser password = 123 hostaddr = 127.0.0.1 port = 5432"
        );

        std::cout << "Writing raw turn data..." << std::endl;
        auto itEdfOuter = edfTurn.cbegin();
        auto it = validTurnHashes.cbegin();
        for (; it != validTurnHashes.cend(); ++it, ++itEdfOuter) {
            const std::vector<long long>& turnAccumI(turnAccum[*it]);
            std::stringstream sql;
            sql << "INSERT INTO " << schemaName << ".turnedf(hash,edf) VALUES (" << *it
                << ",'{";
            auto itEdfInner = itEdfOuter->cbegin();
            sql << *itEdfInner;
            for (++itEdfInner; itEdfInner != itEdfOuter->cend(); ++itEdfInner) {
                sql << "," << *itEdfInner;
            }
            sql << "}');";
            db.work(sql);
        }
    }

    std::cout << "Parsing river data..." << std::endl;
    {
        std::vector<std::vector<float>> edfRiver(validRiverHashes.size());
        auto itEdfOuter = edfRiver.begin();
        for (auto it = validRiverHashes.cbegin(); it != validRiverHashes.cend(); ++it) {
            const std::vector<long long>& riverAccumI(riverAccum[*it]);
            const float rTotal = 1.0/float(riverAccumI.back());
            itEdfOuter->reserve(riverAccumI.size());
            auto itEdfInner = itEdfOuter->begin();
            for (auto itAccum = riverAccumI.cbegin(); itAccum != riverAccumI.cend(); ++itAccum) {
                *itEdfInner = float(*itAccum)*rTotal;
                ++itEdfInner;
            }
            ++itEdfOuter;
        }
        std::cout << "Writing river edf..." << std::endl;

        pqxxInterface db(
            "dbname = dsdata user = dsuser password = 123 hostaddr = 127.0.0.1 port = 5432"
        );

        std::cout << "Writing raw river data..." << std::endl;
        auto itEdfOuter = edfRiver.cbegin();
        auto it = validRiverHashes.cbegin();
        for (; it != validRiverHashes.cend(); ++it, ++itEdfOuter) {
            const std::vector<long long>& riverAccumI(riverAccum[*it]);
            std::stringstream sql;
            sql << "INSERT INTO " << schemaName << ".riveredf(hash,edf) VALUES (" << *it
                << ",'{";
            auto itEdfInner = itEdfOuter->cbegin();
            sql << *itEdfInner;
            for (++itEdfInner; itEdfInner != itEdfOuter->cend(); ++itEdfInner) {
                sql << "," << *itEdfInner;
            }
            sql << "}');";
            db.work(sql);
        }
    }

    return 0;
}