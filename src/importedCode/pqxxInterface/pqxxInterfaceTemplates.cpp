
// ****** Private Member Functions ****** //

template <class T>
static std::vector<T> ds::pqxxInterface::readArrayFromVecTokens(
    VecToken& vt,
    T (vt::*assertGetFn)(),
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    vt.assertGet(openChar);
    do {
        const Token& peekToken(vt.peek());
        if (peekToken == closeChar) {
            break;
        } else if (peekToken == eofToken) {
            FatalError << "Unexpected end of array string. Full string (as tokens) is:\n"
                << vt << std::endl;
            abort();
        }
        vt.push_back((vt.*assertGetFn)());
    } while (it != vt.cend());
}

// ****** END ****** //