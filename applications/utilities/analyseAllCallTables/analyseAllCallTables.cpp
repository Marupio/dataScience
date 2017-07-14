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
    size_t nSeats(inputDict.lookup<size_t>("nSeats"));

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
            sql << "SELECT * from " << schemaName << "." << tableName << " WHERE hand = 'AA';";
//            sql << "AND won = 't' AND flop_rank > 95;";
            pqxx::result R(
                db.result(sql)
            );
            std::ofstream os("AA_fullOutput");
            os << "id won flop_rank turn_rank river_rank "
                << "fp_0 fp_1 fp_2 fp_3 fp_4 fp_5 fp_6 fp_7 fp_8 fp_9 fp_10 "
                << "fp_11 fp_12 fp_13 fp_14 fp_15 fp_16 fp_17 fp_18 fp_19 fp_20 "
                << "fp_21 fp_22 fp_23 fp_24 fp_25 fp_26 fp_27 fp_28 fp_29 fp_30 "
                << "fp_31 fp_32 fp_33 fp_34 fp_35 fp_36 fp_37 fp_38 fp_39 fp_40 "
                << "fp_41 fp_42 fp_43 fp_44 fp_45 fp_46 "
                << "tp_0 tp_1 tp_2 tp_3 tp_4 tp_5 tp_6 tp_7 tp_8 tp_9 tp_10 "
                << "tp_11 tp_12 tp_13 tp_14 tp_15 tp_16 tp_17 tp_18 tp_19 tp_20 "
                << "tp_21 tp_22 tp_23 tp_24 tp_25 tp_26 tp_27 tp_28 tp_29 tp_30 "
                << "tp_31 tp_32 tp_33 tp_34 tp_35 tp_36 tp_37 tp_38 tp_39 tp_40 "
                << "tp_41 tp_42 tp_43 tp_44 tp_45\n";
            long count = 0;
            for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
                std::cout << count++ << " of " << R.size() << std::endl;
                long long id = c[0].as<long long>();
                bool won = c[2].as<bool>();
                short flop_rank = c[3].as<short>();
                short turn_rank = c[4].as<short>();
                short river_rank = c[5].as<short>();
                std::vector<short> flop_predict =
                    pqxxInterface::readArray<short>(c[6].as<std::string>());
                std::vector<short> turn_predict =
                    pqxxInterface::readArray<short>(c[7].as<std::string>());
                os << id << " " << won << " " << flop_rank << " " << turn_rank << " "
                    << river_rank;
                for (auto it = flop_predict.cbegin(); it != flop_predict.cend(); ++it) {
                    os << " " << *it;
                }
                for (auto it = turn_predict.cbegin(); it != turn_predict.cend(); ++it) {
                    os << " " << *it;
                }
                os << std::endl;
            }
        }
    }

    return 0;
}