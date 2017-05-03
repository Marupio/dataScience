#ifndef types_h
#define types_h

#include<pair.h>
#include<vector.h>

namespace ds {

// Typedefs
typedef short DeckInd;
typedef std::vector<DeckInd> VecDeckInd;
typedef std::vector<VecDeckInd> VecVecDeckInd;
typedef std::pair<DeckInd, DeckInd> PktDeckInd;
typedef std::vector<PktDeckInd> VecPktDeckInd;

typedef short CardVal;
typedef std::vector<CardVal> VecCardVal;

typedef float Money;

const static Money SMALL = 1e-6;

class PktVals;
//typedef std::pair<VecCardVal, VecPktVals> StraightCompleters;
//typedef std::vector<std::pair<CardVal, PktVals>> StraightCompleters;

//typedef signed char BinCardVal;

} // end namespace ds

#endif
