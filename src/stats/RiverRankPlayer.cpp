#include <pqxxInterface.h>

#include <RiverRankPlayer.h>
#include <Table.h>
#include <HandRanker.h>

// ****** Constructors ****** //

ds::RiverRankPlayer::RiverRankPlayer()
{}


ds::RiverRankPlayer::RiverRankPlayer
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

void ds::RiverRankPlayer::initialise(
    size_t newId,
    const std::string& name,
    const std::string& schemaName,
    const std::string& tableName
) {
    setIdAndName(newId, name);
    schemaName_ = schemaName;
    tableName_ = tableName;
}


void ds::RiverRankPlayer::observeEvent(eventEnum event) {
    switch (event) {
    case Player::evFlop: {
        HandRanker hr(table().board(), copyPkt_);
        flopRank_ = hr.rank();
        hr.predictAndSort(flopPredict_);
        break;
    }
    case Player::evTurn: {
        HandRanker hr(table().board(), copyPkt_);
        turnRank_ = hr.rank();
        hr.predictAndSort(turnPredict_);
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


void ds::RiverRankPlayer::observeResults() {
    copyPkt_.sort();
    std::string hand(copyPkt_.startingHandName());
    char won;
    if (summary().rewardedMoney > SMALL) {
        won = 't';
    } else {
        won = 'f';
    }

    pqxxInterface db(
        "dbname = dsdata user = dsuser password = 123 hostaddr = 127.0.0.1 port = 5432"
    );

    std::stringstream sql;
    sql << "INSERT INTO " << schemaName_ << "." << tableName_ << "(hand,won,river_rank) VALUES ('"
        << hand << "','" << won << "'," << riverRank_ << ");";
    db.work(sql);
}


// ****** END ****** //
