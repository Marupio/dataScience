#include <Entry.h>


// ****** Constructors ****** //

ds::Entry::Entry(Dictionary& parent, std::istream& is):
    parent_(parent)
{
    if (!getKeyword(is)) {
        return;
    }
    Token nextToken(is);
    nextToken.putBack(is);
    if (nextToken == Token::eof) {
        break;
    } else if (nextToken == Token::openScope) {
        dictPtr_.reset(new Dictionary(keyword, parent, is));
    } else {
        tokensPtr_.reset(new VecToken(is));
        VecToken& vt(*tokensPtr_);
        if (!vt.good()) {
        	FatalError << "Error reading token stream for keyword '" << keyword_ << "' in "
        		<< "Dictionary '" << parent_.name() << "'. Read:\n" << vt << std::endl;
    		abort();
        }
    }
}


// ****** Public Member Functions ****** //

std::string& ds::Entry::keyword() {
    return keyword_;
}


const std::string& ds::Entry::keyword() const {
    return keyword_;
}


ds::Dictionary& ds::Entry::dict() {
    #ifdef DSDEBUG
    if (dictPtr_.get() == nullptr) {
        FatalError << "Attempting to access dictionary on a stream entry." << std::endl;
        abort();
    }
    #endif
    return *dictPtr_;
}


const ds::Dictionary& ds::Entry::dict() const {
    #ifdef DSDEBUG
    if (dictPtr_.get() == nullptr) {
        FatalError << "Attempting to access dictionary on a stream entry." << std::endl;
        abort();
    }
    #endif
    return *dictPtr_;
}


ds::Itstream& ds::Entry::stream() {
    #ifdef DSDEBUG
    if (streamPtr_.get() == nullptr) {
        FatalError << "Attempting to access stream on a dictionary entry." << std::endl;
        abort();
    }
    #endif
    return *streamPtr_;
}


const ds::Itstream& ds::Entry::stream() const {
    #ifdef DSDEBUG
    if (streamPtr_.get() == nullptr) {
        FatalError << "Attempting to access stream on a dictionary entry." << std::endl;
        abort();
    }
    #endif
    return *streamPtr_;
}


bool ds::Entry::isDict() const {
    return dictPtr_.get() != nullptr;
}


bool ds::Entry::isStream() const {
    return streamPtr_.get() != nullptr;
}


bool ds::Entry::isNull() const {
    return !isDict() && !isStream;
}


// ****** Private Member Functions ****** //

bool ds::Entry::getKeyword(std::istream& is) {
    Token nextToken(is);
    if (nextToken.isWord()) {
        keyword_ = token.word();
        return true;
    } else {
        return false;
    }
}


void ds::Entry::readStreamEntry(std::istream& is) {
    if (!read(is)) {
        FatalError << "Bad stream entry format for keyword '" << keyword_ << "' in dictionary '"
            << parent_.name() << "'" << std::endl;
        abort();
    }
}

// ****** END ****** //