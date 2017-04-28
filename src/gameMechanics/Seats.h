#ifndef Seats_h
#define Seats_h

#include<Player.h>

// Forward declarations

typedef VecPlayerPtr::iterator SeatedPlayer;
typedef std::vector<SeatedPlayer> VecSeatedPlayer;
typedef VecPlayerPtr::const_iterator ConstSeatedPlayer;


class Seats {

public:

    // Constructors
    
        //- Construct from components
        Seats(size_t nSeats);
    
    
    // Public Member Functions

        // Access
        
            //- Return the number of seats
            size_t nSeats() const;

            //- Get the number of active players
            size_t nPlayers() const;

            //- Get the number of players leaving the Seats
            size_t nLeaving() const;

            //- Return raw data
            const VecPlayer& seated() const;
            

        // Search
        
            //- Move to the next seated player
            void nextPlayer(SeatedPlayer& st);
            void nextPlayer(ConstSeatedPlayer& cst) const;
            
            //- Move to the next seated player who is not 'waitingForButton'
            void nextActivePlayer(SeatedPlayer& st);
            void nextActivePlayer(ConstSeatedPlayer& cst) const;

            //- Move to the next seated player who still has pocket cards
            void nextCardedPlayer(SeatedPlayer& st);
            void nextCardedPlayer(ConstSeatedPlayer& cst) const;


        // Actions
            
            //- Add player
            void addPlayer(PlayerPtr& player);
            
            //- Add players
            void addPlayers(VecPlayerPtr& players);

            //- Queue player (add to waiting list)
            void queuePlayer(PlayerPtr& player);

            //- Queue players (add to waiting list)
            void queuePlayers(VecPlayerPtr& players);
            
            //- Add players waiting in the queue
            void addQueue();

            //- Kick player:
            //      appends player ID to leavingSeats array and sets
            //      player pointer to nullptr
            void kickPlayer(SeatedPlayer& sp);

            //- Kick players:
            //      appends player IDs to leavingSeats array and sets
            //      player pointers to nullptr
            void kickPlayers(VecSeatedPlayer& vsp);
            
            //- Kicks all players
            void kickAllPlayers();
            
            //- Access leavingSeats buffer
            std::vector<size_t>& leavingSeats();
        

protected:

    // Protected Data

        //- Players seated at the Seats
        //  This is the raw data array
        VecPlayerPtr seated_;

        //- Players waiting to sit down
        VecPlayerPtr seatQueue_;

        //- Players leaving the Seats
        std::vector<size_t> leaving_;



private:

    // Private Data
    
        //- Number of players seated - this is derived data that must be kept
        //  up to date
        size_t nSeated_;
};

#endif
