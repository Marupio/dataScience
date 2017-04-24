// A player interface for stats simulations where players can only choose to
// call or fold.

#ifndef BinaryPlayer_h
#define BinaryPlayer_h

#include<Table.h>

class BinaryPlayer {

public:

    // Constructors
    
        //- Construct with references
        BinaryPlayer(const Table& tbl, const PktCards& pkt);
    
    
    // Public Member Functions

        //- Get the player's action
        virtual bool getAction() = 0;
    


};

#endif
