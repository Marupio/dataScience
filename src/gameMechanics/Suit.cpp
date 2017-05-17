#include <Suit.h>
#include <Card.h>

// ****** Global Functions ****** //

void ds::writeVecSuit(const VecSuit& vs, std::ostream& os) {
    os << "(";
    if (vs.size()) {
        os << Card::suitToWriteChar(vs.front());
        for (
            typename VecSuit::const_iterator it = vs.begin() + 1;
            it != vs.end();
            ++it
        ) {
            os << " " << Card::suitToWriteChar(*it);
        }
    }
    os << ")";
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
                << "read elements are:\n" << vs << std::endl;
            abort();
        }
        char nextChar;
        is >> nextChar;
        vs.push_back(Card::readCharToSuit(nextChar));
    }
    return vs;
}


void ds::writeVecVecSuit(const VecVecSuit& vs, std::ostream& os) {
    os << "\n(\n";
    if (vs.size()) {
        writeVecSuit(vs.front(), os);
        for (
            typename VecVecSuit::const_iterator it = vs.begin() + 1;
            it != vs.end();
            ++it
        ) {
            os << "\n";
            writeVecSuit(*it, os);
        }
    }
    os << "\n)\n";
}


ds::VecVecSuit ds::readVecVecSuit(std::istream& is) {
    VecVecSuit vvs;
    // look for first '('
    while (1) {
        int nxt(is.peek());
        if (std::isspace(nxt)) {
            is.get();
            continue;
        }
        else if (char(nxt) == '(') {
            is.get();
            break;
        } else if (nxt == EOF) {
            FatalError << "Unexpected EOF while reading vector. Successfully "
                << "read elements are:\n" << vvs << std::endl;
            abort();
        }
        else {
            FatalError << "Expecting '(' from istream.  Got '" << char(nxt)
                << "', which is int " << nxt << "." << std::endl;
            abort();
        }
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
                << "read elements are:\n" << vvs << std::endl;
            abort();
        }
        vvs.push_back(readVecSuit(is));
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
                << "read elements are:\n" << vvs << std::endl;
            abort();
        } else {
            FatalError << "Expecting ')' from istream.  Got '" << char(nxt)
                << "', which is int " << nxt << "." << std::endl;
            abort();
        }
    }
    return vvs;
}


void ds::writeSuitCount(const SuitCount& sc, std::ostream& os) {
    os << "(" << short(sc.front());
    for (
        typename SuitCount::const_iterator it = sc.begin() + 1;
        it != sc.end();
        ++it
    ) {
        os << " " << short(*it);
    }
    os << ")";
}


ds::SuitCount ds::readSuitCount(std::istream& is) {
    SuitCount sc;
    char firstChar(is.get());
    if (firstChar != '(') {
        FatalError << "Expecting '(' from istream.  Got '" << firstChar
            << "', which is int " << short(firstChar) << "." << std::endl;
        abort();
    }
    SuitCount::size_type i = 0;
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
                << "read elements are:\n";
            writeSuitCount(sc, std::cerr);
            std::cerr << std::endl;
            abort();
        }
        char nextChar;
        is >> nextChar;
        sc[i++] = nextChar;
    }
    return sc;
}


void ds::writeVecSuitCount(const VecSuitCount& vsc, std::ostream& os) {
    os << "\n(\n";
    if (vsc.size()) {
        writeSuitCount(vsc.front(), os);
        for (
            typename VecSuitCount::const_iterator it = vsc.begin() + 1;
            it != vsc.end();
            ++it
        ) {
            os << "\n";
            writeSuitCount(*it, os);
        }
    }
    os << "\n)\n";
}


ds::VecSuitCount ds::readVecSuitCount(std::istream& is) {
    VecSuitCount vsc;
    // look for first '('
    while (1) {
        int nxt(is.peek());
        if (std::isspace(nxt)) {
            is.get();
            continue;
        }
        else if (char(nxt) == '(') {
            is.get();
            break;
        } else if (nxt == EOF) {
            FatalError << "Unexpected EOF while reading vector. Successfully "
                << "read elements are:\n";
            writeVecSuitCount(vsc, std::cerr);
            std::cerr << std::endl;
            abort();
        }
        else {
            FatalError << "Expecting '(' from istream.  Got '" << char(nxt)
                << "', which is int " << nxt << "." << std::endl;
            abort();
        }
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
                << "read elements are:\n";
            writeVecSuitCount(vsc, std::cerr);
            std::cerr << std::endl;
            abort();
        }
        vsc.push_back(readSuitCount(is));
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
                << "read elements are:\n";
            writeVecSuitCount(vsc, std::cerr);
            std::cerr << std::endl;
            abort();
        } else {
            FatalError << "Expecting ')' from istream.  Got '" << char(nxt)
                << "', which is int " << nxt << "." << std::endl;
            abort();
        }
    }
    return vsc;
}

ds::VecSuit ds::getMissingSuits(const SuitCount& sc) {
    VecSuit missing;
    missing.reserve(2);
    for (SuitCount::const_iterator it = sc.begin(); it != sc.end(); ++it) {
        if (*it == 0) {
            missing.push_back(it - sc.begin());
        }
    }
    return missing;
}

// ****** End ****** //
