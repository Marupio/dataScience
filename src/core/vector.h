#ifndef vector_h
#define vector_h

#include <vector>
#include <error.h>

namespace ds {

template<class T>
inline std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "(";
    typename std::vector<T>::const_iterator it = v.cbegin();
    if (it != v.cend()) {
        os << *it;
        ++it;
    }
    for (; it != v.cend(); ++it) {
        os << " " << *it;
    }
    os << ")";
    return os;
}


template<class T>
std::istream& operator>>(std::istream& is, std::vector<T>& v) {
    while (1) {
        int nxt(is.peek());
        if (std::isspace(nxt)) {
            is.get();
            continue;
        } else {
            break;
        }
    }

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
        T readT;
        is >> readT;
        v.push_back(readT);
    }
    return is;
}

} // End namespace ds
#endif
