#include <Dictionary.h>

// ****** Static Member Data ****** //

ds::Dictionary::Dictionary null;


// ****** Constructors ****** //

ds::Dictionary::Dictionary():
    parent(null)
{}


ds::Dictionary::Dictionary(std::istream& is):
    parent(null)
{
    read(is);
}


ds::Dictionary::Dictionary(const std::string& fileName):
    parent(null),
    name_(fileName)
{
    std::ifstream(fileName) is;
    read(is);
}


ds::Dictionary::Dictionary(const std::string& name, const Dictionary& parent, std::istream& is):
    parent(parent),
    name_(parent.name() + ":" + name)
{

}


const std::string& ds::Dictionary::name() const {
    return name_;
}


void ds::Dictionary::add(Entry&& e) {
    push_back(e);
    hashedEntries_.insert({e.keyword(), &back()});
}


ds::VecToken& ds::Dictionary::lookup(std::string keyword) {
    // ???
}


ds::Dictionary& ds::Dictionary::subDict(std::string keyword) {
    // ???
}




    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const Dictionary& c);
    friend std::istream& operator>>(std::istream& in, Dictionary& c);


// ****** Private Member Functions ****** //

void ds::Dictionary::read(std::istream& is) {
    while (true) {
        Entry e(const std::string& keyword, Dictionary& parent, std::istream& is);
        if (e.isNull()) {
            break;
        }
        add(std::move(e));
    }
}


// ****** END ****** //