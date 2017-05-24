#include <fstream>
#include <Dictionary.h>

// ****** Static Member Data ****** //

const ds::Dictionary ds::Dictionary::null;


// ****** Constructors ****** //

ds::Dictionary::Dictionary():
    parent_(null)
{}


ds::Dictionary::Dictionary(std::istream& is):
    parent_(null)
{
    read(is);
}


ds::Dictionary::Dictionary(const std::string& fileName):
    parent_(null),
    name_(fileName)
{
    std::ifstream is(fileName);
    read(is);
}


ds::Dictionary::Dictionary(const std::string& name, const Dictionary& parent, std::istream& is):
    parent_(parent),
    name_(parent.name() + ":" + name)
{
    read(is);
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


const std::string& ds::Dictionary::name() const {
    return name_;
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
        FatalError << "Cannot find keyword '" << keyword << "' in Dictionary '" << name_ << "'"
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
            << name_ << "'" << std::endl;
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
            << name_ << "'" << std::endl;
        abort();
    }
    return (*it).second->dict();
}


const ds::Dictionary& ds::Dictionary::lookupDict(const std::string& keyword) const {
    auto it = hashedEntries_.find(keyword);
    if (it == hashedEntries_.cend()) {
        FatalError << "Cannot find Dictionary Entry for keyword '" << keyword << "' in Dictionary '"
            << name_ << "'" << std::endl;
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
            FatalError << "Keyword '" << copyKeyword << "' already exists in Dictionary '" << name_
                << "'" << std::endl;
            abort();
        }
    }
    entries_.push_back(std::move(e));
    hashedEntries_.insert({copyKeyword, &entries_.back()});
}


void ds::Dictionary::erase(const std::string& keyword, bool failOnNotFound) {
    auto it = hashedEntries_.find(keyword);
    if (it == hashedEntries_.cend()) {
        if (failOnNotFound) {
            FatalError << "Cannot find Dictionary Entry for keyword '" << keyword << "' in Dictionary '"
                << name_ << "'" << std::endl;
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
    for (auto it = d.cbegin(); it != d.cend(); ++it) {
        os << *it << "\n";
    }
    os << std::endl;
    return os;
}


std::istream& ds::operator>>(std::istream& is, Dictionary& d);


// ****** END ****** //