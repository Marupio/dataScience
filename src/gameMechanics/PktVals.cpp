#include<PktVals.h>
#include<Card.h>

// ****** Static Member Data ****** //

const ds::PktVals ds::PktVals::unknownValues =
    {Card::unknownValue, Card::unknownValue};
const ds::PktVals ds::PktVals::lowAces = {Card::lowAce, Card::lowAce};
const ds::PktVals ds::PktVals::highAces = {Card::ace, Card::ace};


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
    std::pair<CardVal, CardVal>(CardA.value(), CardB.value())
{}


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


ds::CardVal ds::PktVals::highestVal() const {
    return std::max(first, second);
}


ds::CardVal ds::PktVals::highestVal(CardVal avoid) const {
    if (first == avoid) {
        return second;
    } else if (second == avoid) {
        return first;
    }
    return std::max(first, second);
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
