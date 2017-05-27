#include <pqxx/pqxx>

#include <AllCallPlayer.h>
#include <Table.h>
#include <HandRanker.h>

// ****** Constructors ****** //

ds::AllCallPlayer::AllCallPlayer()
{}


ds::AllCallPlayer::AllCallPlayer
(
    size_t id,
    const std::string& name,
    const std::string& schemaName,
    const std::string& tableName
):
    Player(id, name),
    schemaName_(schemaName),
    tableName_(tableName)
{}


// ****** Public Member Functions ****** //

void ds::AllCallPlayer::initialise(
    size_t newId,
    const std::string& name,
    const std::string& schemaName,
    const std::string& tableName
) {
    setIdAndName(newId, name);
    schemaName_ = schemaName;
    tableName_ = tableName;
}


void ds::AllCallPlayer::observeEvent(eventEnum event) {
    switch (event) {
    case Player::evFlop: {
        HandRanker hr(table().board(), copyPkt_);
        flopRank_ = hr.rank();
        hr.predict(flopPredict_);
        break;
    }
    case Player::evTurn: {
        HandRanker hr(table().board(), copyPkt_);
        turnRank_ = hr.rank();
        hr.predict(turnPredict_);
        break;
    }
    case Player::evRiver: {
        HandRanker hr(table().board(), copyPkt_);
        riverRank_ = hr.rank();
        break;
    }
    default: {
        break;
        // Do nothing
    } // end default
    } // end switch
}


void ds::AllCallPlayer::observeResults() {
    copyPkt_.sort();
    std::stringstream hand;
    hand << Card::valueToWriteChar(copyPkt_.first.value())
        << Card::valueToWriteChar(copyPkt_.second.value());
    if (!copyPkt_.pairs()) {
        if (copyPkt_.suited()) {
            hand << 's';
        } else {
            hand << 'u';
        }
    }
    char won;
    if (summary().rewardedMoney > SMALL) {
        won = 't';
    } else {
        won = 'f';
    }
    try {
        pqxx::connection C("dbname = dsdata user = dsuser password = 123 \
        hostaddr = 127.0.0.1 port = 5432");
        if (!C.is_open()) {
            FatalError << "Can't open database, player id " << summary().id << ", '"
                << summary().name << "'" << std::endl;
            abort();
        }
        std::stringstream sql;
        sql << "INSERT INTO " << schemaName_ << "." << tableName_ << "(hand,won,flop_rank,turn_rank"
            << ",river_rank,flop_predict,turn_predict) VALUES ('" << hand.str() << "','" << won
            << "'," << flopRank_ << "," << turnRank_ << "," << riverRank_ << ",'{";
        auto itf = flopPredict_.cbegin();
        sql << *itf;
        for (++itf; itf != flopPredict_.cend(); ++itf) {
            sql << "," << *itf;
        }
        sql << "}','{";
        auto itt = turnPredict_.cbegin();
        sql << *itt;
        for (++itt; itt != turnPredict_.cend(); ++itt) {
            sql << "," << *itt;
        }
        sql << "}');";
        pqxx::work W(C);
        W.exec(sql.str().c_str());
        W.commit();
        C.disconnect ();
    } catch (const std::exception &e) {
        FatalError << "Database write failure, player id " << summary().id << ", '"
            << summary().name << "', error: '" << e.what() << "'" << std::endl;
        abort();
    }
}


// ****** END ****** //
