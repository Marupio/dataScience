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

    std::string tableName("riverrank_" + std::to_string(nSeats));

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

    // Output data is collected into probs:
    //  probs[chsi][riverRank] = probability of winning
    //  Where:
    //  * chsi (compact starting hand index) is 0..168 with no gaps
    //  * shi (starting hand index) is given by:
    //      PktCards::chsiToShi[chsi] = shi
    std::vector<std::vector<Scalar>> probs(169, std::vector<Scalar>(1226, -1.));
    std::vector<std::vector<long long>> counts(169, std::vector<long long>(1226, 0));

    std::ofstream os("analyseRiverRankTables_output");

    for (short i = 0; i < 169; ++i) {
        std::vector<long long>& countsI(counts[i]);
        std::vector<Scalar>& probsI(probs[i]);
        PktCards pkt(PktCards::makeStartingHand(i));
        std::string hand = pkt.startingHandName();
        std::cout << "Working on " << hand << std::endl;
        os << hand << "_prob " << hand << "_count ";
        std::stringstream sql;
        sql << "SELECT * from " << schemaName << "." << tableName << " WHERE hand = '" << hand
            << "';";
        pqxx::result R(
            db.result(sql)
        );
        std::array<long long, 1225> nWonByRank;
        nWonByRank.fill(0);
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            bool won = c[2].as<bool>();
            short river_rank = c[3].as<short>();
            countsI[river_rank]++;
            if (won) {
                nWonByRank[river_rank]++;
            }
        }
        auto itP = probsI.begin();
        auto itC = countsI.cbegin();
        auto itW = nWonByRank.cbegin();
        while (itP != probsI.end()) {
            *itP = Scalar(*itW)/Scalar(*itC);
            ++itP;
            ++itC;
            ++itW;
        }
    }
    os << "\n";
    for (short rank = 0; rank < 1226; ++rank) {
        for (short cshi = 0; cshi < 169; ++cshi) {
            os << probs[cshi][rank] << " " << counts[cshi][rank] << " ";
        }
        os << "\n";
    }

    return 0;
}