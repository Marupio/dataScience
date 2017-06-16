#include <iterator>
#include <fstream>
#include <functional>
#include <list>
#include <thread>

#include <pqxxInterface.h>

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

    {
        pqxxInterface db(
            "dbname = dsdata user = dsuser password = 123 hostaddr = 127.0.0.1 port = 5432"
        );

        if (!db.foundSchema(schemaName)) {
            FatalError << "Schema '" << schemaName << "' does not exist."
                << std::endl;
            abort();
        }

        if (!db.foundSchemaTable(schemaName, tableName)) {
            FatalError << "Table '" << schemaName << "." << tableName << "' does not exist."
                << std::endl;
            abort();
        }

        {
            std::stringstream sql;
            sql << "SELECT * from " << schemaName << "." << tableName << " WHERE hand = 'AA'";
            sql << "AND won = 't' AND flop_rank > 95;";
            pqxx::result R(
                pqxx.result(sql)
            );
            for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
                std::cout << "ID = " << c[0].as<long long>() << "\n"
                    << "g = " << c[1].as<std::string>() << "\n"
                    << "won =" << c[2].as<bool>() << "\n"
                    << "flop_rank = " << c[3].as<short>() << "\n"
                    << "turn_rank = " << c[4].as<short>() << "\n"
                    << "river_rank = " << c[5].as<short>() << "\n"
                    << "flop_predict = " << c[6].as<std::string>() << "\n"
                    << "turn_predict = " << c[7].as<std::string>() << std::endl;
            }
        }
    }

    return 0;
}