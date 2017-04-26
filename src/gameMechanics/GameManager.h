#ifndef GameManager_h
#define GameManager_h

#include<list>
#include<Table.h>
#include<vector.h>

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

            // TODO
            // Buy in, rake, rewards
            

        // Active data

            //- Master player list
            std::vector<Player> players_;
            
            //- Table list
            std::list<Table> tables_;
};

#endif
