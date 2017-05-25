
template<class T>
void ds::Dictionary::addType(std::string& keyword, const T& val, bool overwrite) {
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
