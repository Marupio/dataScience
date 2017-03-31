#include<Suit.h>
#include<Card.h>

// ****** Global Functions ****** //

void ds::writeVecSuit(const VecSuit& vs, std::ostream& os) {
    os << "(";
    if (v.size()) {
        os << Card::suitToWriteChar(v.front());
        for (
            typename std::vector<T>::const_iterator it = v.begin() + 1;
            it != v.end();
            ++it
        ) {
            os << " " << Card::suitToWriteChar(*it);
        }
    }
    os << ")";
    return os;
}


ds::VecSuit ds::readVecSuit(std::istream& is) {
    VecSuit vs;
    char firstChar(is.get());
    if (firstChar != '(') {
        FatalError << "Expecting '(' from istream.  Got '" << firstChar
            << "', which is int " << short(firstChar) << "." << std::endl;
        abort();
    }
    while (1) {
        int nxt(is.peek());
        if (std::isspace(nxt)) {
            is.get();
            continue;
        }
        else if (char(nxt) == ')') {
            is.get();
            break;
        } else if (nxt == EOF) {
            FatalError << "Unexpected EOF while reading vector. Successfully "
                << "read elements are:\n" << v << std::endl;
            abort();
        }
        vs.push_back(Card::readCharToSuit(char(is)));
    }
    return vs;
}


void ds::writeVecVecSuit(const VecVecSuit& vs, std::ostream& os) {
    os << "(\n";
    if (vs.size()) {
        writeVecSuit(vs.front());
        for (
            typename std::vector<T>::const_iterator it = v.begin() + 1;
            it != v.end();
            ++it
        ) {
            os << "\n";
            writeVecSuit(*it, os);
        }
    }
    os << "\n)";
    return os;
}


ds::VecVecSuit readVecVecSuit(std::istream& is) {
    VecVecSuit vs;
    char firstChar(is.get());
    if (firstChar != '(') {
        FatalError << "Expecting '(' from istream.  Got '" << firstChar
            << "', which is int " << short(firstChar) << "." << std::endl;
        abort();
    }
    while (1) {
        int nxt(is.peek());
        if (std::isspace(nxt)) {
            is.get();
            continue;
        }
        else if (char(nxt) == ')') {
            is.get();
            break;
        } else if (nxt == EOF) {
            FatalError << "Unexpected EOF while reading vector. Successfully "
                << "read elements are:\n" << v << std::endl;
            abort();
        }
        vs.push_back(readVecSuit(is));
    }
    // Look for closing ')'
    while (1) {
        int nxt(is.peek());
        if (std::isspace(nxt)) {
            is.get();
            continue;
        }
        else if (char(nxt) == ')') {
            is.get();
            break;
        } else if (nxt == EOF) {
            FatalError << "Unexpected EOF while reading vector. Successfully "
                << "read elements are:\n" << v << std::endl;
            abort();
        }
    }
    return vs;
}

