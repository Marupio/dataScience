// Ghost player left behind by a player leaving with a fast-fold
// If any action is requested, it folds
#ifndef GhostPlayer_h
#define GhostPlayer_h

#include<Player.h>

namespace ds {

class GhostPlayer: public Player {

public:


    // Constructors
    
        //- Construct from components
        GhostPlayer(const Player::Summary& summary):
            Player(summary)
        {}
    
    
    // Public Member Functions
        
        // Action
        
            //- Bet option for player interface, see Player.h for more details
            virtual Player::Decision betOption(
                Money totalBet,
                Money minRaise
            ) {
                return Player::Decision(acFold, -1);
            }

            //- GhostPlayer can only call or fold, see Player.h for more details
            virtual Player::Decision callFoldOption(
                Money totalBet
            ) {
                return Player::Decision(acFold, -1);
            }
};

} // end namespace

#endif
