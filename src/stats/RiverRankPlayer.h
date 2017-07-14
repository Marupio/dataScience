// Calls down to the river, saves hand, river rank, and won to database
// Manages stack size
#ifndef RiverRankPlayer_h
#define RiverRankPlayer_h

#include <Player.h>

namespace ds {

class RiverRankPlayer: public Player {

public:

    // Constructors
    
        //- Construct null
        RiverRankPlayer();
    
        //- Construct from components
        RiverRankPlayer(
            size_t id,
            const std::string& name,
            const std::string& schemaName,
            const std::string& tableName
        );


    // Public Member Functions

        // Initialisation

            //- Initialise with id, name, schema and table name
            void initialise(
                size_t newId,
                const std::string& name,
                const std::string& schemaName,
                const std::string& tableName
            );


        // Action
        
            //- Bet option for player interface
            virtual Player::Decision betOption(
                Money totalBet,
                Money minRaise
            ) {
                return Player::Decision(Player::acCall, -1);
            }

            //- Player can only call or fold
            virtual Player::Decision callFoldOption(
                Money totalBet
            ) {
                return Player::Decision(Player::acCall, -1);
            }

            //- Inform player that an event has occurred
            //  Default does nothing
            virtual void observeEvent(eventEnum event);

            //- Called at the end of a hand of poker to allow players to take
            //  note of the results, default does nothing
            virtual void observeResults();


private:

    // Private Member Functions


    // Private Member Data

        //- Database schema and table names
        std::string schemaName_;
        std::string tableName_;

        //- Transient data
        short flopRank_;
        short turnRank_;
        short riverRank_;
        std::vector<short> flopPredict_;
        std::vector<short> turnPredict_;
};

} // end namespace

#endif
