#include <Entry.h>
#include <Dictionary.h>
#include <VecToken.h>

// ****** Constructors ****** //

ds::Entry::Entry(Dictionary& parent, std::istream& is, bool requireSemiColonEnd):
    type_(teUnknown),
    parent_(parent)
{
    if (!getKeyword(is)) {
        return;
    }
    read(is, requireSemiColonEnd);
}


ds::Entry::Entry(
    Dictionary& parent,
    const std::string& keyword,
    std::istream& is,
    bool requireSemiColonEnd
):
    keyword_(keyword),
    type_(teUnknown),
    parent_(parent)
{
    read(is, requireSemiColonEnd);
}


ds::Entry::Entry(Dictionary& parent, Dictionary&& subDict):
    keyword_(subDict.keyName()),
    type_(teDict),
    dictPtr_(new Dictionary(std::move(subDict))),
    parent_(parent)
{}


// ****** Public Member Functions ****** //

std::string& ds::Entry::keyword() {
    return keyword_;
}


const std::string& ds::Entry::keyword() const {
    return keyword_;
}


ds::Entry::typeEnum ds::Entry::type() const {
    return type_;
}


ds::Dictionary& ds::Entry::dict() {
    #ifdef DSDEBUG
    if (dictPtr_.get() == nullptr) {
        FatalError << "Dictionary is not allocated in Entry. Type is: '"
            << typeEnumToString(type_) << "'" << std::endl;
        abort();
    }
    #endif
    return *dictPtr_;
}


const ds::Dictionary& ds::Entry::dict() const {
    #ifdef DSDEBUG
    if (dictPtr_.get() == nullptr) {
        FatalError << "Dictionary is not allocated in Entry. Type is: '"
            << typeEnumToString(type_) << "'" << std::endl;
        abort();
    }
    #endif
    return *dictPtr_;
}


const ds::VecToken& ds::Entry::tokens() const {
    #ifdef DSDEBUG
    if (type_ != teTokens) {
        FatalError << "Attempting to access tokens on Entry with type '" << typeEnumToString(type_)
            << "'" << std::endl;
        abort();
    }
    #endif
    return tokens_;
}


std::string ds::Entry::typeEnumToString(typeEnum te) {
    switch (te)
    {
        case teUnknown: return "Unknown";
        case teEof: return "Eof";
        case teDict: return "Dictionary";
        case teTokens: return "Tokens";
        default:
        {
            FatalError << "Unknown typeEnum: " << te << std::endl;
            abort();
        }
    }
    return std::string();
}


bool ds::Entry::isDict() const {
    return type_ == teDict;
}


bool ds::Entry::isTokens() const {
    return type_ == teTokens;
}


bool ds::Entry::isEof() const {
    return type_ == teEof;
}


bool ds::Entry::isNull() const {
    return !isDict() && !isTokens();
}


void ds::Entry::debugWrite(std::ostream& os) const {
    os << "parentName = '" << parent_.name() << "' ";
	switch (type_) {
	case teUnknown: {
		os << "UnknownEntryType";
		break;
	}
	case teEof: {
		break;
	}
	case teDict: {
		os << "subDictionary '" << keyword_ << "'\n";
		dictPtr_->debugWrite(os);
		break;
	}
	case teTokens: {
		os << "tokens '" << keyword_ << "' ";
		tokens_.debugWrite(os);
		break;
	}
	default: {
		FatalError << "Unknown Entry type: " << type_ << std::endl;
		abort();
	} // end default
	} // end switch
}



// ****** Private Member Functions ****** //

void ds::Entry::read(std::istream& is, bool requireSemiColonEnd) {
    Token nextToken(is);
    if (nextToken == Token::Eof) {
        type_ = teEof;
        nextToken.putBack(is);
        return;
    } else if (nextToken == Token::openScope) {
        type_ = teDict;
        dictPtr_.reset(new Dictionary(keyword_, parent_, is));
    } else {
        nextToken.putBack(is);
        VecToken::eofBehaviourEnum eb =
            requireSemiColonEnd ? VecToken::ebFailOnEof : VecToken::ebAllowEof;
        tokens_.read(is, eb, VecToken::rbFailIfNotEmpty);
        if (!tokens_.good()) {
            FatalError << "Error reading token stream for keyword '" << keyword_ << "' in "
                << "Dictionary '" << parent_.name() << "'. Read:\n" << tokens_ << std::endl;
            abort();
        }
        type_ = teTokens;
    }
}

bool ds::Entry::getKeyword(std::istream& is) {
    Token nextToken(is);
    if (nextToken.isWord()) {
        keyword_ = nextToken.getWord();
        return true;
    } else {
        return false;
    }
}


// ****** Global operators ****** //

std::ostream& ds::operator<<(std::ostream& os, const Entry& e) {
    int nspaces = ((e.parent_.depth()) + 1)*Dictionary::tabWidth;
    os << std::string(nspaces, ' ');
	switch (e.type_) {
    	case Entry::teUnknown: {
    		os << e.keyword_ << " UnknownEntryType";
    		break;
    	}
    	case Entry::teEof: {
    		break;
    	}
    	case Entry::teDict: {
    		os << e.keyword_ << "\n" << *e.dictPtr_;
    		break;
    	}
    	case Entry::teTokens: {
    		os << e.keyword_ << " " << e.tokens_ << Token::semiColon;
    		break;
    	}
    	default: {
    		FatalError << "Unknown Entry type: " << e.type_ << std::endl;
    		abort();
    	} // end default
	} // end switch
    return os;
}


// ****** END ****** //