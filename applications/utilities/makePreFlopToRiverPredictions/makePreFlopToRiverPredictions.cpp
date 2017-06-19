#include <pqxxInterface.h>

#include <dsConfig.h>
#include <types.h>
#include <Deck.h>
#include <DeckMask.h>
#include <HandRanker.h>
#include <PktCards.h>

std::string schemaName("staticData");
std::string tableName("preFlopToRiver");

void writePreFlopData(PktCards& pkt, pqxxInterface& db, int chen) {
    Board bd;
    HandRanker hr(bd, pkt);
    std::vector<short> pa = hr.predictPreFlopToRiver();
    std::stringstream sql;
                {"id", "bigserial primary key"},
                {"hand", "varchar(3) not null"},
                {"chen", "smallint not null"},
                {"preflop_to_river", "smallint[] not null"}
    sql << "INSERT INTO " << schemaName << "." << tableName << "(hand,chen,preflop_to_river) "
        << "VALUES ('" << hand << "','" << chen << ",'{";
    auto it = pa.cbegin();
    sql << *it;
    for (++it; it != pa.cend(); ++it) {
        sql << "," << *it;
    }
    sql << "}');";
    db.work(sql);
}


int main(int argc, char *argv[]) {
    {
        pqxxInterface db(
            "dbname = dsdata user = dsuser password = 123 hostaddr = 127.0.0.1 port = 5432"
        );

        if (!db.foundSchema(schemaName)) {
            db.createSchema(schemaName);
        }

        bool tableExists = db.foundSchemaTable(schemaName, tableName);
        if (tableExists) {
            db.dropTable(schemaName, tableName);
        }

        std::vector<std::pair<std::string, std::string>> headingsAndTypes =
            {
                {"id", "bigserial primary key"},
                {"hand", "varchar(3) not null"},
                {"chen", "smallint not null"},
                {"preflop_to_river", "smallint[] not null"}
            };
        db.createTable(schemaName, tableName, headingsAndTypes);

        for (CardVal cvA = Card::ace; cvA > Card::lowAce; --cvA) {
            std::cout << "Working on " << Card::valueToWriteChar(cvA) << std::endl;
            for (CardVal cvB = cvA; cvB > Card::lowAce; --cvB) {
                int chen = 0;
                if (cvA <= 10) {
                    chen = cvA/2;
                } else if (cvA == Card::ace) {
                    chen = 10;
                } else if (cvA == Card::king) {
                    chen = 8;
                } else if (cvA == Card::queen) {
                    chen = 7;
                } else if (cvA == Card::jack) {
                    chen = 6;
                }
                short delta = cvA - cvB;
                switch (delta) {
                case 0:
                    chen *= 2;
                    // fall through
                case 1:
                    if (cvA < Card::king) {
                        chen += 1;
                    }
                    break;
                case 2:
                    chen -= 1;
                    break;
                case 3:
                    chen -= 2;
                    break;
                case 4:
                    chen -= 4;
                    break;
                default:
                    chen -= 5;
                    break;
                } // end switch
                
                // Unsuited first
                {
                    PktCards pkt(cvA, Card::clubs, cvB, Card::diamonds);
                    std::string handType = pkt.handType();
                    writePreFlopData(pkt, db, chen);
                }
                if (cvA != cvB) {
                    // Suited next
                    chen += 2;
                    PktCards pkt(cvA, Card::clubs, cvB, Card::clubs);
                    std::string handType = pkt.handType();
                    writePreFlopData(pkt, db, chen);
                }
            }
        }
    }
    return 0;
}


// ****** END ****** //