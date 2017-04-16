#include<PktCards.h>

// ****** Constructors ****** //

ds::PktCards::PktCards():
    std::pair<Card, Card>(Card::unknownCard, Card::unknownCard)
{}


ds::PktCards::PktCards(const Card& cardA, const Card& cardB):
    std::pair<Card, Card>(cardA, cardB)
{}


ds::PktCards::PktCards
(
    CardVal valA,
    Suit suitA,
    CardVal valB,
    Suit suitB
):
    std::pair<Card, Card>(Card(valA, suitA), Card(valB, suitB))
{}


ds::PktCards::PktCards
(
    const Card& CardA,
    CardVal valB,
    Suit suitB
):
    std::pair<Card, Card>(CardA, Card(valB, suitB))
{}


ds::PktCards::PktCards
(
    CardVal valA,
    Suit suitA,
    const Card& CardB
):
    std::pair<Card, Card>(Card(valA, suitA), CardB)
{}


ds::PktCards::PktCards(DeckInd diA, DeckInd diB):
    std::pair<Card, Card>(Card(diA), Card(diB))
{}


ds::PktCards::PktCards(VecDeckInd vdi):
    std::pair<Card, Card>(Card::unknownCard, Card::unknownCard)
{
    if (vdi.size() != 2) {
        FatalError << "Attempting to construct PktCards from "
            << "VecDeckInd with size " << vdi.size() << std::endl;
        abort();
    }
    first = Card(vdi.front());
    second = Card(vdi.back());
}


ds::PktCards::PktCards(std::istream& is) {
    is >> *this;
}


// ****** Member Functions ****** //

bool ds::PktCards::has(const Card& cd) const {
    return first == cd || second == cd;
}


bool ds::PktCards::has(CardVal cv) const {
    return first.value() == cv || second.value() == cv;
}


bool ds::PktCards::has(Suit st) const {
    return first.suit() == st || second.suit() == st;
}


bool ds::PktCards::suited() const {
    return first.suit() == second.suit();
}


bool ds::PktCards::suited(Suit st) const {
    return first.suit() == st && second.suit() == st;
}


bool ds::PktCards::pairs() const {
    return first.value() == second.value();
}


bool ds::PktCards::pairs(CardVal cv) const {
    return first.value() == cv && second.value() == cv;
}


ds::CardVal ds::PktCards::highestVal() const {
    return std::max(first.value(), second.value());
}


ds::CardVal ds::PktCards::highestVal(CardVal avoid) const {
    if (first.value() == avoid) {
        return second.value();
    } else if (second.value() == avoid) {
        return first.value();
    }
    return std::max(first.value(), second.value());
}


void ds::PktCards::swap() {
    Card temp = first;
    first = second;
    second = temp;
}


void ds::PktCards::sort() {
    if (first.value() < second.value()) {
        swap();
    }
}


ds::PktVals ds::PktCards::values() const {
    return PktVals(first.value(), second.value());
}


ds::PktVals ds::PktCards::sortedValues() const {
    if (first.value() >= second.value()) {
        return PktVals(first.value(), second.value());
    }
    return PktVals(second.value(), first.value());
}


// ****** Member Operators ****** //
    
bool ds::PktCards::operator==(const PktCards& pc) const {
    return (
        first == pc.first && second == pc.second
    ) || (
        second == pc.first && first == pc.second
    );
}


bool ds::PktCards::operator!=(const PktCards& pc) {
    return !(operator==(pc));
}


ds::PktCards::operator VecCard() {
    return {first, second};
}


// ****** Global Functions ****** //

bool ds::noWildEquals(const PktCards& pcA, const PktCards& pcB) {
    return (
        noWildEquals(pcA.first, pcB.first)
     && noWildEquals(pcA.second, pcB.second)
    ) || (
        noWildEquals(pcA.first, pcB.second)
     && noWildEquals(pcA.second, pcB.first)
    );
}

// ****** END ****** //
