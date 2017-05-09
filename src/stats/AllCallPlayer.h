// Virtual interface for a player at the table
// Manages stack size
#ifndef AllCallPlayer_h
#define AllCallPlayer_h

#include<Player.h>

namespace ds {

class AllCallPlayer: public Player {

public:

    // Constructors
    
        //- Construct null
        AllCallPlayer();
    
        //- Construct from components
        AllCallPlayer(size_t id, const std::string& name);


    // Public Member Functions

        // Access
        
            //- Access databases
            const std::vector<unsigned long long>& nWon() const;
            const std::vector<unsigned long long>& nLost() const;
            const std::vector<unsigned long long>& winningFlopRankSum() const;
            const std::vector<unsigned long long>& winningTurnRankSum() const;
            const std::vector<unsigned long long>& winningRankSum() const;
            const std::vector<unsigned long long>& losingFlopRankSum() const;
            const std::vector<unsigned long long>& losingTurnRankSum() const;
            const std::vector<unsigned long long>& losingRankSum() const;

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

        //- Transient data
        short flopRank_;
        short turnRank_;
        short riverRank_;

        //- Databases
        std::vector<unsigned long long> nWon_;
        std::vector<unsigned long long> nLost_;
        std::vector<unsigned long long> winningFlopRankSum_;
        std::vector<unsigned long long> winningTurnRankSum_;
        std::vector<unsigned long long> winningRankSum_;
        std::vector<unsigned long long> losingFlopRankSum_;
        std::vector<unsigned long long> losingTurnRankSum_;
        std::vector<unsigned long long> losingRankSum_;
};

} // end namespace

#endif
