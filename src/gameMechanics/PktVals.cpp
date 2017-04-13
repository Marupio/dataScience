#include<PktVals.h>
#include<Card.h>

// ****** Static Member Data ****** //

static const PktVals unknownValues = {Card::unknownValue, Card::unknownValue};
static const PktVals lowAces = {Card::lowAce, Card::lowAce};
static const PktVals highAces = {Card::ace, Card::ace};


// ****** Constructors ****** //

ds::PktVals::PktVals():
    std::pair<CardVal, CardVal>(
        Card::unknownValue,
        Card::unknownValue
    )
{}


ds::PktVals::PktVals(const CardVal& valA, const CardVal& valB):
    std::pair<CardVal, CardVal>(valA, valB)
{}


ds::PktVals::PktVals
(
    const Card& CardA,
    const Card& CardB
):
    std::pair<CardVal, CardVal>(CardA.value(), Card.value())
{}


ds::PktVals::PktVals(DeckInd diA, DeckInd diB):
    std::pair<CardVal, CardVal>(Card(diA).value(), Card(diB).value())
{}


ds::PktVals::PktVals(VecDeckInd vdi):
    std::pair<CardVal, CardVal>(Card::unknownValue, Card::unknownValue)
{
    if (vdi.size() != 2) {
        FatalError << "Attempting to construct PktVals from "
            << "VecDeckInd with size " << vdi.size() << std::endl;
        abort();
    }
    first = Card(vdi.front()).value();
    second = Card(vdi.back()).value();
}


ds::PktVals::PktVals(std::istream& is) {
    is >> *this;
}


// ****** Member functions ****** //

bool ds::PktVals::has(CardVal cv) const {
    return first == cv || second == cv;
}


bool ds::PktVals::pairs() const {
    return first == second;
}


bool ds::PktVals::pairs(CardVal cv) const {
    return first == cv && second == cv;
}


void ds::PktVals::swap() {
    CardVal temp = first;
    first = second;
    second = temp;
}


void ds::PktVals::sort() {
    if (first < second) {
        swap();
    }
}


// ****** Member Operators ****** //

bool ds::PktVals::operator==(const PktVals& pc) const {
    return (
        first == pc.first && second == pc.second
    ) || (
        second == pc.first && first == pc.second
    );
}


bool ds::PktVals::operator!=(const PktVals& pc) {
    return !(operator==(pc));
}
        

ds::PktVals::operator VecCardVal() {
    return {first, second};
}


// ****** END ****** //
