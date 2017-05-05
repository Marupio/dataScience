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
        GhostPlayer(const Table& table, const Player::Summary& summary):
            Player(table, summary)
        {}
    
    
    // Public Member Functions
        
        // Action
        
            //- Bet option for player interface, see Player.h for more details
            virtual std::pair<actionEnum, Money> betOption(
                Money totalBet,
                Money minRaise
            ) {
                return std::pair<actionEnum, Money>(acFold, -1);
            }

            //- GhostPlayer can only call or fold, see Player.h for more details
            virtual std::pair<actionEnum, Money> callFoldOption(
                Money totalBet
            ) {
                return std::pair<actionEnum, Money>(acFold, -1);
            }

            //- Inform player that an event has occurred
            virtual void observeEvent(eventEnum event) {
                // Do nothing
            }

            virtual void observeAction(
                const SeatedPlayer& player,
                actionEnum action,
                Money amount
            ) {
                // Do nothing
            }

            virtual void observeResults() {
                // Do nothing
            }
};

} // end namespace

#endif
