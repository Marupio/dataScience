// A reference to a single player

#ifndef PlayerRef_h
#define PlayerRef_h

#include<utility>
#include<vector> 

namespace ds {

// Forward declarations
class Player;

typedef std::pair<int, Player*> PlayerRef;
typedef std::vector<PlayerRef> VecPlayerRef;

} // end namespace ds

#endif
