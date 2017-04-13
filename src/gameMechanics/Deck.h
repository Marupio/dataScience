#ifndef Deck_h
#define Deck_h

#include<array>
#include<iterator>
#include<types.h>

namespace ds {

//class Deck;
//std::ostream& operator<<(std::ostream& os, const Deck& c);
//std::istream& operator>>(std::istream& in, Deck& c);


class Deck
{
public:

    // Public typedefs
    typedef
        std::iterator_traits<
            std::array<DeckInd, 52>::const_iterator
        >::difference_type size_type;

    // Constructors

        //- Construct null
        //  Randomise - when true, resets the standard randomisation seed
        //  to the clock time
        Deck(bool randomise = false);

//        //- Construct from istream
//        Deck(std::istream& is);


    //- Destructor
    ~Deck() {}


    // Member functions

        //- Shuffle the cards
        void shuffle();

        //- Draw one card
        DeckInd draw();
        
        //- Draw n cards
        VecDeckInd draw(short n);

        //- Return nRemaining
        size_type nRemaining() const noexcept;




//    // Operators
//    
//    // Friend functions
//    friend std::ostream& operator<<(std::ostream& out, const Deck& c);
//    friend std::istream& operator>>(std::istream& in, Deck& c);


private:

    // Private member data

        //- Cards in deck
        std::array<DeckInd, 52> cards_;
        
        //- Current position in deck
        std::array<DeckInd, 52>::const_iterator drawIt_;


    // Private member functions

};

} // end namespace ds

#endif
