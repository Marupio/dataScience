#ifndef GameManager_h
#define GameManager_h

#include <list>
#include <Table.h>
#include <vector.h>
#include <Player.h>

// Forward declarations

class GameManager {

public:


    // Constructors
    
        //- Construct from components
        GameManager();
    
    
    // Public Member Functions

        //- Main execution function
        void execute();
        

private:

    // Private Data

        // Settings
        
            //- Blinds levels
            std::vector<Blinds> blindsLevels_;
            
            //- Duration [seconds]
            //  0 means unlimited
            int duration_;
            
            //- Max number of players
            int nPlayersMax_;
            
            //- Seats per table
            size_t seatsPerTable_;


        // Active data

            //- Master player list
            std::vector<Player*> players_;
            
            //- Table list
            std::list<Table> tables_;
};

#endif
