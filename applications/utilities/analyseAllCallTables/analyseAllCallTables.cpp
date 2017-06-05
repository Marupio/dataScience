#include <iterator>
#include <fstream>
#include <functional>
#include <list>
#include <thread>

#include <pqxx/pqxx>

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
        if (!schemaExists) {
            throw dsException("Schema '" + schemaName + "' does not exist.\n");
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
        if (!tableExists) {
            throw dsException("Table '" + schemaName + "." + tableName + "' does not exist.\n");
        }

        // if (!tableExists || overwrite) {
        //     std::stringstream sql;
        //     sql << "CREATE TABLE " << schemaName << "." << tableName << " ("
        //         << "id              bigserial primary key, "
        //         << "g               varchar(3) not null, "
        //         << "won             bool not null, "
        //         << "flop_rank       smallint not null, "
        //         << "turn_rank       smallint not null, "
        //         << "river_rank      smallint not null, "
        //         << "flop_predict    smallint[] not null, "
        //         << "turn_predict    smallint[] not null "
        //         << ")";
        //     pqxx::work W(C);
        //     W.exec(sql.str().c_str());
        //     W.commit();
        // }

        {
            std::stringstream sql;
            sql << "SELECT * from " << schemaName << "." << tableName << " WHERE hand = 'AA'";
            sql << "AND won = 't' AND flop_rank > 95;";
            pqxx::nontransaction N(C);
            pqxx::result R( N.exec( sql ));
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

        C.disconnect ();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}