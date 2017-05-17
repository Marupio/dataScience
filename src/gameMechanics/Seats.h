#ifndef Seats_h
#define Seats_h

#include <mutex>
#include <atomic>
#include <GhostPlayer.h>

namespace ds {

class Seats {

public:

    // Constructors
    
        //- Construct null
        Seats();
    
        //- Construct from components
        Seats(size_t nSeats);

    
    // Public Member Functions

        // Access

            //- Return the number of seats
            size_t nSeats() const;

            //- Get the number of active players
            size_t nPlayers() const;
            
            //- How many players can be added to the queue
            size_t roomInQueue() const;


        // Actions
            
            //- Add player
            //  ** Not thread safe **
            //  For use only before play has started
            void addPlayer(PlayerPtr& player);
            
            //- Add players
            //  ** Not thread safe **
            //  For use only before play has started
            void addPlayers(VecPlayerPtr& players);

            //- Queue player (add to waiting list)
            void queuePlayer(PlayerPtr& player);

            //- Queue players (add to waiting list)
            void queuePlayers(VecPlayerPtr& players);
            
            //- Transfer waitingToLeave_ to left
            void takeLeavingPlayers(std::vector<size_t>& left);
        

protected:

    // Protected Member Functions

        // Access
    
            //- Return raw data
            const VecPlayerPtr& seated() const;


        // Search
        
            //- Returns first seated player, starting from st
            SeatedPlayer firstPlayer(SeatedPlayer& st);
            ConstSeatedPlayer firstPlayer(ConstSeatedPlayer& st) const;
        
            //- Move to the next seated player
            void nextPlayer(SeatedPlayer& st);
            void nextPlayer(ConstSeatedPlayer& cst) const;
            
            //- Returns first seated player who is not 'waitingForButton',
            //  starting from st
            SeatedPlayer firstActivePlayer(SeatedPlayer& st);
            ConstSeatedPlayer firstActivePlayer(ConstSeatedPlayer& st) const;
        
            //- Move to the next seated player who is not 'waitingForButton'
            void nextActivePlayer(SeatedPlayer& st);
            void nextActivePlayer(ConstSeatedPlayer& cst) const;

            //- Returns first seated player who still has pocket cards,
            //  starting from st
            SeatedPlayer firstCardedPlayer(SeatedPlayer& st);
            ConstSeatedPlayer firstCardedPlayer(ConstSeatedPlayer& st) const;
        
            //- Move to the next seated player who still has pocket cards
            void nextCardedPlayer(SeatedPlayer& st);
            void nextCardedPlayer(ConstSeatedPlayer& cst) const;

            //- Move to the next empty seat
            void nextEmptySeat(SeatedPlayer& st);
            void nextEmptySeat(ConstSeatedPlayer& cst) const;
    

        // Actions

            //- Add players waiting in the queue
            void addQueue();

            //- Kick player:
            //      appends player ID to waitingToLeave array and sets player
            //      pointer to nullptr
            void kick(SeatedPlayer& sp);

            //- Kick players:
            //      appends player IDs to waitingToLeave array and sets player
            //      pointers to nullptr
            void kick(VecSeatedPlayer& vsp);
            
            //- Kicks all players
            void kickAllPlayers();

            //- Ghost kick player - for fastFolds
            //      appends player ID to waitingToLeave array and sets player
            //      pointer to a new local ghostPlayer that has the same
            //      Summary as the kicked player
            void ghostKick(SeatedPlayer& sp);
            
            //- Remove all ghost players from the seats
            void clearGhostPlayers();
            
            //- Check the waitingToSit, and if players are there, sit them at
            //  any available seats starting from the dealer
            void seatWaitingPlayers(SeatedPlayer dealer);


    // Protected Data

        //- Number of seats
        std::atomic<size_t> nSeats_;

        //- Number of players seated - this is derived data that must be kept
        //  up to date
        std::atomic<size_t> nPlayers_;

        //- Players seated at the Seats
        //  This is the raw data array
        VecPlayerPtr seated_;

        //- Players waiting to sit down
        VecPlayerPtr waitingToSit_;
        mutable std::mutex waitingToSitMutex_;

        //- Players leaving the Seats
        std::vector<size_t> waitingToLeave_;
        mutable std::mutex waitingToLeaveMutex_;


private:

    // Private Data
    
        //- Ghost players left behind from fast-folded players
        std::vector<GhostPlayer> ghostPlayers_;
        
        //- The seats occupied by ghostPlayers
        VecSeatedPlayer ghostPlayerSeats_;
};


} // end namespace
#endif
