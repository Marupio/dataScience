#ifndef pair_h
#define pair_h

#include <utility>

namespace ds {

template<class T1, class T2>
inline std::ostream& operator<<(
    std::ostream& os,
    const std::pair<T1, T2>& p
) {
    os << "[" << p.first << " " << p.second << "]";
    return os;
}


template<class T1, class T2>
inline std::istream& operator>>(std::istream& is, std::pair<T1, T2>& p) {
    while (1) {
        int nxt(is.peek());
        if (std::isspace(nxt)) {
            is.get();
            continue;
        } else if (nxt == EOF) {
            FatalError << "Unexpected EOF while reading pair." << std::endl;
            abort();
        } else {
            break;
        }
    }
    char firstChar;
    is >> firstChar;
    if (firstChar != '[') {
        FatalError << "Expecting '[' while reading pair. Got '" << firstChar
            << "', which is int " << int(firstChar) << std::endl;
        abort();
    }
    is >> p.first;
    while (1) {
        int nxt(is.peek());
        if (std::isspace(nxt)) {
            is.get();
            continue;
        } else if (nxt == EOF) {
            FatalError << "Unexpected EOF while reading pair." << std::endl;
            abort();
        } else {
            break;
        }
    }
    is >> p.second;
    char lastChar;
    is >> lastChar;
    if (lastChar != ']') {
        FatalError << "Expecting ']' while reading pair. Got '" << lastChar
            << "', which is int " << int(lastChar) << std::endl;
        abort();
    }
    return is;
}

} // End namespace ds
#endif
