
template<class T>
T ds::Dictionary::lookup(std::string keyword) {
    const VecToken& vt = lookupTokens(keyword);
    T ret;
    std::stringstream ss;
    ss << vt;
    ss >> ret;
    return ret;
}


template<class T>
T ds::Dictionary::lookupOrDefault(std::string keyword, T defaultVal) {
    if (found(keyword)) {
        const VecToken& vt = lookupTokens(keyword);
        T ret;
        std::stringstream ss;
        ss << vt;
        ss >> ret;
        return ret;
    } else {
        return defaultVal;
    }
}


template<class T>
void ds::Dictionary::addType(std::string keyword, const T& val, bool overwrite) {
    auto it = hashedEntries_.find(keyword);
    if (it == hashedEntries_.end()) {
        if (overwrite) {
            erase(keyword);
        } else {
            FatalError << "Cannot find Dictionary Entry for keyword '" << keyword << "' in "
                << "Dictionary '" << name() << "'" << std::endl;
            abort();
        }
    }

    std::stringstream ss;
    ss << val;
    Entry e(*this, ss, false);

    entries_.push_back(std::move(e));
    hashedEntries_.insert({e.keyword(), &entries_.back()});
}
