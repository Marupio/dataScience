#ifndef types_h
#define types_h

#include<utility>
#include<vector>

namespace ds {

// Typedefs
typedef short DeckInd;
typedef std::vector<DeckInd> VecDeckInd;
typedef std::vector<VecDeckInd> VecVecDeckInd;
typedef std::pair<DeckInd, DeckInd> PktDeckInd;
typedef std::vector<PktDeckInd> VecPktDeckInd;

typedef short CardVal;
typedef std::vector<CardVal> VecCardVal;
typedef std::pair<CardVal, CardVal> PktVals;
typedef std::vector<PktVals> VecPktVals;

typedef std::pair<VecCardVal, VecPktVals> StraightCompleters;

typedef signed char Suit;
typedef signed char BinCardVal;

}
#endif