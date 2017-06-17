
// ****** Private Member Functions ****** //

template <class T>
std::vector<T> ds::pqxxInterface::readArrayFromVecTokens(
    VecToken& vt,
    T (VecToken::*assertGetFn)() const,
    char openChar,
    char closeChar,
    char delim
) {
    std::vector<T> res;
    vt.assertGet(openChar);
    {
        const Token& peekToken(vt.peek());
        if (peekToken == closeChar) {
            return res;
        } else if (peekToken == Token::eofToken) {
            FatalError << "Unexpected end of array string. Full string (as tokens) is:\n"
                << vt << std::endl;
            abort();
        }
        res.push_back((vt.*assertGetFn)());
    }
    while (true) {
        const Token& peekToken(vt.peek());
        if (peekToken == closeChar) {
            break;
        } else if (peekToken == Token::eofToken) {
            FatalError << "Unexpected end of array string. Full string (as tokens) is:\n"
                << vt << std::endl;
            abort();
        }
        vt.assertGet(delim);
        res.push_back((vt.*assertGetFn)());
    }
    return res;
}

// ****** END ****** //