#ifndef Avoiders_h
#define Avoiders_h

#include<types.h>
#include<PktVals.h>


namespace ds {

struct AvoidPktVals {
    AvoidPktVals(const PktVals& p): avoid(p) {}
    AvoidPktVals(CardVal cA, CardVal cB): avoid(cA, cB) {}
    PktVals avoid;
    bool operator()(CardVal testVal) const {
        return (testVal == avoid.first || testVal == avoid.second);
    }
};


struct AvoidValue {
    AvoidValue(CardVal p): avoid(p) {}
    CardVal avoid;
    bool operator()(CardVal testVal) const {
        return (testVal == avoid);
    }
};


struct AvoidNone {
    AvoidNone() {}
    bool operator()(CardVal testVal) const {
        return false;
    }
};

} // end namespace ds

#endif
