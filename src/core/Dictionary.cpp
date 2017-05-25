#include <fstream>
#include <sstream>
#include <Dictionary.h>

// ****** Static Member Data ****** //

const ds::Dictionary ds::Dictionary::null;

int ds::Dictionary::tabWidth(4);


// ****** Constructors ****** //

ds::Dictionary::Dictionary():
    parent_(null),
    depth_(0)
{}


ds::Dictionary::Dictionary(std::istream& is):
    parent_(null),
    depth_(0)
{
    read(is);
}


ds::Dictionary::Dictionary(const std::string& fileName):
    parent_(null),
    keyName_(fileName),
    depth_(0)
{
    std::ifstream is(fileName);
    read(is);
}


ds::Dictionary::Dictionary(const std::string& name, const Dictionary& parent, std::istream& is):
    parent_(parent),
    scopeName_(parent.name()),
    keyName_(name),
    depth_(parent.depth() + 1)
{
    read(is);
}


ds::Dictionary::Dictionary(const Dictionary& dict):
    parent_(null),
    depth_(0)
{
    std::stringstream ss;
    ss << dict;
    read(ss);
}


ds::Dictionary::Dictionary(Dictionary&& dict):
    parent_(null),
    depth_(0)
{
    if (dict.depth_ > 0) {
        // Use copy constructor, then remove from parent
        *this = dict;
        Dictionary& dictParent(const_cast<Dictionary&>(dict.parent_));
        dictParent.erase(dict.keyName_);
    } else {
        scopeName_.swap(dict.scopeName_);
        keyName_.swap(dict.keyName_);
        entries_.swap(dict.entries_);
        hashedEntries_.swap(dict.hashedEntries_);
    }
}


ds::Dictionary ds::Dictionary::operator=(const Dictionary& dict)
{
    return Dictionary(dict);
}


ds::Dictionary ds::Dictionary::operator=(Dictionary&& dict)
{
    return Dictionary(std::move(dict));
}


// ****** Public Member Functions ****** //


ds::Dictionary::const_iterator ds::Dictionary::cbegin() const noexcept {
    return entries_.cbegin();
}


ds::Dictionary::const_iterator ds::Dictionary::cend() const noexcept {
    return entries_.cend();
}


ds::Dictionary::const_reverse_iterator ds::Dictionary::crbegin() const noexcept {
    return entries_.crbegin();
}


ds::Dictionary::const_reverse_iterator ds::Dictionary::crend() const noexcept {
    return entries_.crend();
}


ds::Dictionary::size_type ds::Dictionary::size() const noexcept {
    return entries_.size();
}


const ds::Dictionary& ds::Dictionary::parent() const {
    return parent_;
}


std::string ds::Dictionary::name() const {
    return scopeName_ + ":" + keyName_;
}


const std::string& ds::Dictionary::scopeName() const {
    return scopeName_;
}


const std::string& ds::Dictionary::keyName() const {
    return keyName_;
}


int ds::Dictionary::depth() const {
    return depth_;
}


bool ds::Dictionary::found(const std::string& keyword) const {
    if (hashedEntries_.find(keyword) == hashedEntries_.cend()) {
        return false;
    }
    return true;
}


ds::Entry::typeEnum ds::Dictionary::foundType(const std::string& keyword) const {
    auto it = hashedEntries_.find(keyword);
    if (it == hashedEntries_.cend()) {
        return Entry::teUnknown;
    }
    return (*it).second->type();
}


const ds::Entry& ds::Dictionary::lookup(const std::string& keyword) const {
    auto it = hashedEntries_.find(keyword);
    if (it == hashedEntries_.cend()) {
        FatalError << "Cannot find keyword '" << keyword << "' in Dictionary '" << name() << "'"
            << std::endl;
        abort();
    }
    return *(*it).second;
}

bool ds::Dictionary::isTokens(const std::string& keyword) const {
    auto it = hashedEntries_.find(keyword);
    if (it == hashedEntries_.cend()) {
        return false;
    }
    return (*it).second->isTokens();
}


const ds::VecToken& ds::Dictionary::lookupTokens(const std::string& keyword) const {
    auto it = hashedEntries_.find(keyword);
    if (it == hashedEntries_.cend()) {
        FatalError << "Cannot find Tokens Entry for keyword '" << keyword << "' in Dictionary '"
            << name() << "'" << std::endl;
        abort();
    }
    return (*it).second->tokens();
}


bool ds::Dictionary::isDict(const std::string& keyword) const {
    auto it = hashedEntries_.find(keyword);
    if (it == hashedEntries_.cend()) {
        return false;
    }
    return (*it).second->isDict();
}


ds::Dictionary& ds::Dictionary::lookupDict(const std::string& keyword) {
    auto it = hashedEntries_.find(keyword);
    if (it == hashedEntries_.end()) {
        FatalError << "Cannot find Dictionary Entry for keyword '" << keyword << "' in Dictionary '"
            << name() << "'" << std::endl;
        abort();
    }
    return (*it).second->dict();
}


const ds::Dictionary& ds::Dictionary::lookupDict(const std::string& keyword) const {
    auto it = hashedEntries_.find(keyword);
    if (it == hashedEntries_.cend()) {
        FatalError << "Cannot find Dictionary Entry for keyword '" << keyword << "' in Dictionary '"
            << name() << "'" << std::endl;
        abort();
    }
    return (*it).second->dict();
}


void ds::Dictionary::add(Entry&& e, bool overwrite) {
    std::string copyKeyword(e.keyword());
    auto it = hashedEntries_.find(copyKeyword);
    if (it != hashedEntries_.end()) {
        if (overwrite) {
            erase(copyKeyword);
        } else {
            FatalError << "Keyword '" << copyKeyword << "' already exists in Dictionary '" << name()
                << "'" << std::endl;
            abort();
        }
    }
    entries_.push_back(std::move(e));
    hashedEntries_.insert({copyKeyword, &entries_.back()});
}


void ds::Dictionary::add(const std::string& keyword, std::istream& is, bool overwrite) {
    Entry e(*this, keyword, is);
    add(std::move(e), overwrite);
}


void ds::Dictionary::add(const std::string& keyword, std::string& parseThis, bool overwrite) {
    std::stringstream is(parseThis);
    Entry e(*this, keyword, is);
    add(std::move(e), overwrite);
}


void ds::Dictionary::add(const std::string& parseThis, bool overwrite) {
    std::stringstream is(parseThis);
    Entry e(*this, is);
    add(std::move(e), overwrite);
}


void ds::Dictionary::add(const std::string& keyword, Dictionary&& subdict, bool overwrite) {
    Entry e(*this, std::move(subdict));
    add(std::move(e), overwrite);
}


void ds::Dictionary::erase(const std::string& keyword, bool failOnNotFound) {
    auto it = hashedEntries_.find(keyword);
    if (it == hashedEntries_.cend()) {
        if (failOnNotFound) {
            FatalError << "Cannot find Dictionary Entry for keyword '" << keyword << "' in Dictionary '"
                << name() << "'" << std::endl;
            abort();
        } else {
            return;
        }
    }
    for (auto ite = entries_.cbegin(); ite != entries_.cend(); ++ite) {
        if (ite->keyword() == keyword) {
            entries_.erase(ite);
            break;
        }
    }
    hashedEntries_.erase(it);
}


void ds::Dictionary::clear() {
    hashedEntries_.clear();
    entries_.clear();
}


void ds::Dictionary::debugWrite(std::ostream& os) const {
    for (auto it = cbegin(); it != cend(); ++it) {
        it->debugWrite(os);
        os << "\n";
    }
    os << std::endl;
}


// ****** Private Member Functions ****** //

void ds::Dictionary::read(std::istream& is) {
    while (true) {
        Entry e(*this, is, true);
        if (e.isNull()) {
            break;
        }
        add(std::move(e));
    }
}


// ****** Global Operators ****** //

std::ostream& ds::operator<<(std::ostream& os, const Dictionary& d) {
    int nspaces = Dictionary::tabWidth*d.depth_;
    os << std::string(nspaces, ' ') << "{\n";
    for (auto it = d.cbegin(); it != d.cend(); ++it) {
        os << *it << "\n";
    }
    os << std::string(nspaces, ' ') << '}';
    return os;
}


std::istream& ds::operator>>(std::istream& is, Dictionary& d) {
    if (d.entries_.size()) {
        FatalError << "Dictionary " << d.name() << "non-empty" << std::endl;
        abort();
    }
    d = Dictionary(is);
    return is;
}


// ****** END ****** //