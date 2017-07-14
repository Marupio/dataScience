#include <PktCards.h>

// ****** Public Static Data ****** //

const std::array<size_t, 169> ds::PktCards::cshiToShi = {
    0, 12, 13, 24, 25, 26, 36, 37, 38, 39, 48, 49, 50, 51, 52, 60, 61, 62, 63, 64, 65, 72, 73, 74,
    75, 76, 77, 78, 84, 85, 86, 87, 88, 89, 90, 91, 96, 97, 98, 99, 100, 101, 102, 103, 104, 108,
    109, 110, 111, 112, 113, 114, 115, 116, 117, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
    130, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 169, 170, 181, 182, 183, 193, 194, 195, 196, 205, 206,
    207, 208, 209, 217, 218, 219, 220, 221, 222, 229, 230, 231, 232, 233, 234, 235, 241, 242, 243,
    244, 245, 246, 247, 248, 253, 254, 255, 256, 257, 258, 259, 260, 261, 265, 266, 267, 268, 269,
    270, 271, 272, 273, 274, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 289, 290, 291,
    292, 293, 294, 295, 296, 297, 298, 299, 300
};


const std::array<size_t, 301> ds::PktCards::shiToCshi = {
    0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1, 2, 1000, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 3, 4, 5, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
    1000, 1000, 6, 7, 8, 9, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 10, 11, 12, 13, 14,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 15, 16, 17, 18, 19, 20, 1000, 1000, 1000, 1000, 1000,
    1000, 21, 22, 23, 24, 25, 26, 27, 1000, 1000, 1000, 1000, 1000, 28, 29, 30, 31, 32, 33, 34, 35,
    1000, 1000, 1000, 1000, 36, 37, 38, 39, 40, 41, 42, 43, 44, 1000, 1000, 1000, 45, 46, 47, 48,
    49, 50, 51, 52, 53, 54, 1000, 1000, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 1000, 66, 67,
    68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 92, 93, 1000, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 94, 95, 96, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
    1000, 1000, 97, 98, 99, 100, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 101, 102, 103, 104,
    105, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 106, 107, 108, 109, 110, 111, 1000, 1000, 1000,
    1000, 1000, 1000, 112, 113, 114, 115, 116, 117, 118, 1000, 1000, 1000, 1000, 1000, 119, 120,
    121, 122, 123, 124, 125, 126, 1000, 1000, 1000, 1000, 127, 128, 129, 130, 131, 132, 133, 134,
    135, 1000, 1000, 1000, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 1000, 1000, 146, 147,
    148, 149, 150, 151, 152, 153, 154, 155, 156, 1000, 157, 158, 159, 160, 161, 162, 163, 164, 165,
    166, 167, 168
};


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


ds::PktCards::PktCards(VecToken& vt) {
    std::stringstream ss;
    ss << vt;
    ss >> *this;
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


ds::CardVal ds::PktCards::highestValue() const {
    return std::max(first.value(), second.value());
}


ds::CardVal ds::PktCards::highestValue(CardVal avoid) const {
    if (first.value() == avoid) {
        return second.value();
    } else if (second.value() == avoid) {
        return first.value();
    }
    return std::max(first.value(), second.value());
}


std::string ds::PktCards::startingHandName() const {
    std::stringstream hand;
    hand << Card::valueToWriteChar(first.value())
        << Card::valueToWriteChar(second.value());
    if (!pairs()) {
        if (suited()) {
            hand << 's';
        } else {
            hand << 'u';
        }
    }
    return hand.str();
}


size_t ds::PktCards::startingHandIndex() const {
    PktVals sorted(sortedValues());
    size_t res = (sorted.first - 2)*12 + sorted.second - 2;
    if (suited()) {
        res += 145;
    }
    return res;
}


size_t ds::PktCards::compactStartingHandIndex() const {
    size_t shi = startingHandIndex();
    return shiToCshi[shi];
}


ds::PktCards ds::PktCards::makeStartingHand(short compactStartingHandIndex) {
    size_t shi = cshiToShi[compactStartingHandIndex];
    // shi = (Big-2)*12+(Small-2)+145*Suited
    Suit secondSuit = Card::hearts;
    if (shi > 156) {
        secondSuit = Card::clubs;
        shi -= 145;
    }
    CardVal small = shi%13 + 2;
    CardVal big = shi/13 + 2;
    return PktCards(big, Card::clubs, small, secondSuit);
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
