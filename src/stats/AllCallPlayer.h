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
            const std::vector<std::vector<short>>& winningFlopRank() const;
            const std::vector<std::vector<short>>& winningTurnRank() const;
            const std::vector<std::vector<short>>& winningRiverRank() const;
            const std::vector<std::vector<short>>& losingFlopRank() const;
            const std::vector<std::vector<short>>& losingTurnRank() const;
            const std::vector<std::vector<short>>& losingRiverRank() const;
            const std::vector<std::vector<std::vector<short>>>&
                winningFlopPredict() const;
            const std::vector<std::vector<std::vector<short>>>&
                winningTurnPredict() const;
            const std::vector<std::vector<std::vector<short>>>&
                losingFlopPredict() const;
            const std::vector<std::vector<std::vector<short>>>&
                losingTurnPredict() const;

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
        std::vector<short> flopPredict_;
        std::vector<short> turnPredict_;

        //- Databases
        std::vector<std::vector<short>> winningFlopRank_;
        std::vector<std::vector<short>> winningTurnRank_;
        std::vector<std::vector<short>> winningRiverRank_;
        std::vector<std::vector<short>> losingFlopRank_;
        std::vector<std::vector<short>> losingTurnRank_;
        std::vector<std::vector<short>> losingRiverRank_;

        std::vector<std::vector<std::vector<short>>> winningFlopPredict_;
        std::vector<std::vector<std::vector<short>>> winningTurnPredict_;
        std::vector<std::vector<std::vector<short>>> losingFlopPredict_;
        std::vector<std::vector<std::vector<short>>> losingTurnPredict_;
};

} // end namespace

#endif
