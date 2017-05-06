#include<AllInPlayer.h>
#include<Table.h>

// ****** Constructors ****** //

ds::AllInPlayer::AllInPlayer(
    const Table& table,
    size_t id,
    const std::string& name
):
    Player(table, id, name)
{
    // Allocate stats tables
}

void ds::AllInPlayer::observeEvent(eventEnum event) {
    
}

void ds::AllInPlayer::observeResults() {
}

protected:

    // Protected Member Functions
    
        //- Look at pocket
        const PktCards& pocket() const;


    // Protected Member Data
    
        //- Reference to the table
        const Table& table_;


private:

    // Private Member Functions

        // Forced actions - friend Table, Seats interface
        
            //- Set waitingForButton
            void setWaitingForButton(bool newValue);

            //- Collect ante or blinds
            //  Returns actual amount collected
            Money collect(Money amount);

            //- Deal cards to this player
            void dealPocket(VecDeckInd cardIndices);

            //- Show cards
            //  Copies pkt_ to revealedPkt_
            void showPocket();

            //- Return cards to dealer
            void clearPocket();

            //- Reward player with a pot, putting it into rewardedMoney for show
            void reward(Money amount);

            //- Transfer rewardedMoney into stack, clear allIn if set
            void takeRewards();

            //- Take back an overbet (when other players don't have that much)
            void returnExcess(Money amount);
            
            //- Remove pushed money
            void clearPushedMoney();

            //- Reset for next round:
            //      * clearPocket
            //      * takeRewards
            void reset();


        // Internal functions
        
            //- Interpret result of reveal[Winning|Losing]CardsOption()
            void parseRevealOption(int answer);

        

    // Private Member Data

        //- Player summary
        Summary summary_;
    
        //- Pocket cards
        PktCards pkt_;
};

} // end namespace

#endif
