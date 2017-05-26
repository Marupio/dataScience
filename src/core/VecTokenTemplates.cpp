
template<class T>
void ds::VecToken::assertGet(T t) const {
    const Token& nextToken(get());
    if (nextToken == t) {
        return;
    }
    FatalError << "Expecting '" << t << "', got '" << nextToken << "'. Full token list is:\n"
        << *this << std::endl;
    abort();
}
