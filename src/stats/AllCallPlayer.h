// Virtual interface for a player at the table
// Manages stack size
#ifndef AllCallPlayer_h
#define AllCallPlayer_h

#include <Player.h>

namespace ds {

class AllCallPlayer: public Player {

public:

    // Constructors
    
        //- Construct null
        AllCallPlayer();
    
        //- Construct from components
        AllCallPlayer(
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
