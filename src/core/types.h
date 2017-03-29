#ifndef types_h
#define types_h


namespace ds {

// Forward declarations
template<class T1, class T2> struct std::pair;

template< class T > struct std::allocator;
template<class T, class Allocator = std::allocator<T>> class std::vector;

template< class Key > struct std::hash;
template< class T > struct std::equal_to;
template<
    class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator< std::pair<const Key, T> >
> class std::unordered_map;


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
