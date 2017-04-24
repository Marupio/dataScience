#include<cstdlib>
#include<numeric>
#include<algorithm>
#include<ctime>
#include<Deck.h>


// ****** Constructors ****** //

ds::Deck::Deck(bool randomise) {
    std::iota(cards_.begin(), cards_.end(), 0);
    drawIt_ = cards_.begin();
    if (randomise) {
        std::srand(std::time(0));
    }
}


// ****** Public member functions ****** //

void ds::Deck::shuffle() {
    std::random_shuffle(cards_.begin(), cards_.end());
    drawIt_ = cards_.begin();
}


ds::DeckInd ds::Deck::draw() {
    if (drawIt_ == cards_.cend()) {
        FatalError << "No more cards remaining." << std::endl;
        abort();
    }
    DeckInd drawn = *drawIt_;
    ++drawIt_;
    return drawn;
}

ds::VecDeckInd ds::Deck::draw(short n) {
    size_type remaining = nRemaining();
    if (remaining < n) {
        FatalError << "Only " << remaining << " cards remain in deck. "
            << "Attempting to draw " << n << "." << std::endl;
        abort();
    }
    VecDeckInd drawnCards;
    drawnCards.reserve(n);
    for (; n > 0; --n) {
        drawnCards.push_back(*drawIt_);
        ++drawIt_;
    }
    return drawnCards;
}


ds::Deck::size_type ds::Deck::nRemaining() const noexcept {
    return std::distance(drawIt_, cards_.cend());
}


// ****** Global operators ****** //

//std::ostream& ds::operator<<(std::ostream& os, const Deck& b) {
//    os << b.cards_;
//    return os;
//}
//
//std::istream& ds::operator>>(std::istream& is, Deck& b) {
//    if (b.cards_.size()) {
//        FatalError << "Reading into non-empty Deck. Deck cards are:\n"
//            << b << std::endl;
//        abort();
//    }
//    is >> b.cards_;
//    return is;
//}

// ****** END ****** //
