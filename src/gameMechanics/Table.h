#ifndef Table_h
#define Table_h

#include<Board.h>
#include<Blinds.h>

class Table {

public:

    // Public Data Types
    
    enum statusEnum {
        seEmpty,
        sePaused,
        sePreFlop,
        seFlop,
        seTurn,
        seRiver,
    };

    // Constructors
    
        //- Construct with references
        Table(const Board& bd);
    
    
    // Public Member Functions

        // Game manager interface
        
            // Queries
            
                //- Get the current status
                statusEnum status() const;
                
                //- Get the number of active players
                size_t nPlayers() const;

                //- Get the number of players leaving the table
                size_t nLeaving() const;

                //- Query current blinds
                const Blinds& currentBlinds();
                

            // Actions        
                
                //- Start / resume play
                void play();

                //- Finish current hand and halt play until released
                void pause();
                
                //- Finish current hand and remove all players from the table
                void disband();

                //- Add player
                void addPlayer(std::shared_ptr<Player> player);
                
                //- Add players
                void addPlayers(std::vector<std::shared_ptr<Player>>& players);
            
                //- Access leavingTable buffer
                std::vector<std::shared_ptr<Player>>& leavingTable();

                //- Change blinds
                void setBlinds(const Blinds& newBlinds);


private:

    // Private Data
    
        //- Reference to board
        const Board& bd_;

        //- Players seated at the table
        std::vector<std::shared_ptr<Player>, nSeats> seated_;

        //- Players leaving the table
        std::vector<std::shared_ptr<Player>, nSeats> leaving_;

        //- Position of the dealer button, indexed according to seated_
        size_t dealer_;
        
        //- Current blinds
        const Blinds* blinds_;
        
        //- Next blinds
        const Blinds* nextBlinds_;
};

#endif
