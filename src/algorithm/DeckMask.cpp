#include<algorithm>
#include<numeric>
#include<DeckMask.h>

// ****** Constructors ****** //

ds::DeckMask::DeckMask() {
    initialiseIndices();
}


ds::DeckMask::DeckMask(const Board& bd) {
    initialiseIndices();
    remove(bd);
}


ds::DeckMask::DeckMask(const PktCards& pkt) {
    initialiseIndices();
    remove(pkt);
}


ds::DeckMask::DeckMask(const Board& bd, const PktCards& pkt) {
    initialiseIndices();
    remove(bd);
    remove(pkt);
}


ds::DeckMask::DeckMask(const VecDeckInd& vdi) {
    initialiseIndices();
    remove(vdi);
}


// ****** Public Member Functions ****** //

short ds::DeckMask::remove(const Board& bd) {
    short nRemoved = 0;
    for (auto it = bd.cbegin(); it != bd.cend(); ++it) {
        nRemoved += remove(it->deckIndex());
    }
    return nRemoved;
}


short ds::DeckMask::remove(const PktCards& pkt) {
    short nRemoved = remove(pkt.first.deckIndex());
    nRemoved += remove(pkt.second.deckIndex());
    return nRemoved;
}


short ds::DeckMask::remove(const Card& c) {
    return remove(c.deckIndex());
}


short ds::DeckMask::remove(const VecDeckInd& vdi) {
    short nRemoved = 0;
    for (auto it = vdi.cbegin(); it != vdi.cend(); ++it) {
        nRemoved += remove(*it);
    }
    return nRemoved;
}


short ds::DeckMask::remove(DeckInd di) {
    #ifdef DS_DEBUG
    if (di > 51 || di < 0) {
        FatalError << "Deck index (" << di << ") out of bounds 0..51"
            << std::endl;
        abort();
    }
    #endif
    auto it = std::lower_bound(begin(), end(), di);
    if (it != end()) {
        erase(it);
        return 1;
    }
    return 0;
}


// ****** Private Member Functions ****** //

void ds::DeckMask::initialiseIndices() {
    clear();
    resize(52);
    std::iota(begin(), end(), 0);
}


// ****** END ****** //
