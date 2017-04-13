#include<PktVals.h>
#include<Card.h>

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

void ds::PktVals::swap() {
    CardVal temp = first;
    first = second;
    second = temp;
}


// ****** Member operators ****** //
    
        //- True if both pocket cards are the same as the given
        bool operator==(const PktVals& pc) const {
            return (
                first == pc.first && second == pc.second
            ) || (
                second == pc.first && first == pc.second
            );
        }

        //- False if either pocket cards differ from given
        bool operator!=(const PktVals& pc) {
            return !(operator==(pc));
        }
        
        //- Cast to VecCardVal
        operator VecCardVal() {
            return {first, second};
        }


    // Friend functions
//    friend std::ostream& operator<<(std::ostream& out, const PktVals& c);
//    friend std::istream& operator>>(std::istream& in, PktVals& c);
};

// ****** END ****** //
