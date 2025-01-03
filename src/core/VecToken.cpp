#include <VecToken.h>

// ****** Constructors ****** //

ds::VecToken::VecToken():
    eofFail_(false),
    readFail_(false),
    readPos_(0)
{}

ds::VecToken::VecToken(std::istream& is):
    eofFail_(false),
    readFail_(false),
    readPos_(0)
{
    read(is);
}


// ****** Public Member Functions ****** //

bool ds::VecToken::good() const {
    return !eofFail_ && !readFail_;
}


bool ds::VecToken::eofFail() const {
    return eofFail_;
}


bool ds::VecToken::readFail() const {
    return readFail_;
}


std::vector<ds::Token>::const_iterator ds::VecToken::readPos() const {
    size_t readPosCopy(readPos_);
    std::vector<Token>::const_iterator ret = cbegin();
    if (ret == cend() && readPos_) {
        FatalError << "Read position set to " << readPos_ << " in VecToken of size " << size()
            << std::endl;
        abort();
    }
    while (readPosCopy--) {
        ++ret;
        if (ret == cend() && readPosCopy) {
            FatalError << "Read position set to " << readPos_ << " in VecToken of size " << size()
                << std::endl;
            abort();
        }
    }
    return ret;
}


const ds::Token& ds::VecToken::peek() const {
    auto it = readPos();
    if (it == cend()) {
        return Token::eofToken;
    }
    return *it;
}


const ds::Token& ds::VecToken::get() const {
    auto it = readPos();
    if (it == cend()) {
        return Token::eofToken;
    }
    readPos_++;
    return *it;
}


void ds::VecToken::assertSize(size_t n) const {
    if (size() != n) {
        FatalError << "Expecting " << n << " tokens, got " << size() << ". They are:\n" << *this
            << std::endl;
        abort();
    }
}


ds::Token::punctuationEnum ds::VecToken::assertGetPunctuation() const {
    const Token& nextToken(get());
    if (!nextToken.isPunctuation()) {
        FatalError << "Expecting punctuation, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getPunctuation();
}


bool ds::VecToken::assertGetBool() const {
    const Token& nextToken(get());
    if (!nextToken.isBool()) {
        FatalError << "Expecting boolean, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getBool();
}



std::string ds::VecToken::assertGetWord() const {
    const Token& nextToken(get());
    if (!nextToken.isWord()) {
        FatalError << "Expecting word, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getWord();
}



std::string ds::VecToken::assertGetString() const {
    const Token& nextToken(get());
    if (!nextToken.isString()) {
        FatalError << "Expecting string, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getString();
}



short ds::VecToken::assertGetShort() const {
    const Token& nextToken(get());
    if (!nextToken.isInt()) {
        FatalError << "Expecting short, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getShort();
}



int ds::VecToken::assertGetInt() const {
    const Token& nextToken(get());
    if (!nextToken.isInt()) {
        FatalError << "Expecting int, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getInt();
}



long ds::VecToken::assertGetLong() const {
    const Token& nextToken(get());
    if (!nextToken.isInt()) {
        FatalError << "Expecting long, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getLong();
}



long long ds::VecToken::assertGetLongLong() const {
    const Token& nextToken(get());
    if (!nextToken.isInt()) {
        FatalError << "Expecting long long, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getLongLong();
}



unsigned short ds::VecToken::assertGetUShort() const {
    const Token& nextToken(get());
    if (!nextToken.isUInt()) {
        FatalError << "Expecting ushort, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getUShort();
}



unsigned int ds::VecToken::assertGetUInt() const {
    const Token& nextToken(get());
    if (!nextToken.isUInt()) {
        FatalError << "Expecting uint, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getUInt();
}



unsigned long ds::VecToken::assertGetULong() const {
    const Token& nextToken(get());
    if (!nextToken.isUInt()) {
        FatalError << "Expecting ulong, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getULong();
}



unsigned long long ds::VecToken::assertGetULongLong() const {
    const Token& nextToken(get());
    if (!nextToken.isUInt()) {
        FatalError << "Expecting ulonglong, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getULongLong();
}



float ds::VecToken::assertGetFloat() const {
    const Token& nextToken(get());
    if (!nextToken.isFloat()) {
        FatalError << "Expecting float, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getFloat();
}



double ds::VecToken::assertGetDouble() const {
    const Token& nextToken(get());
    if (!nextToken.isFloat()) {
        FatalError << "Expecting double, got '" << nextToken << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
        abort();
    }
    return nextToken.getDouble();
}



void ds::VecToken::unget() const {
    if (!readPos_) {
        FatalError << "Attempting to unget at beginning of VecToken" << std::endl;
        abort();
    }
    readPos_--;
}


void ds::VecToken::rewind() const {
    readPos_ = 0;
}


void ds::VecToken::read(
    std::istream& is,
    eofBehaviourEnum eb,
    readBehaviourEnum rb
) {
    if (size()) {
        switch (rb) {
        case rbOverwrite:
            this->clear();
            this->shrink_to_fit();
            eofFail_ = false;
            readFail_ = false;
            break;
        case rbFailIfNotEmpty: {
            FatalError << "Attempting to read token stream into non-empty VecToken. Contains:\n"
                << *this << std::endl;
            abort();
        }
        case rbAppend:
            if (!good()) {
                FatalError << "Attempting to append to VecToken with failure flags set: "
                    << "eofFail=" << eofFail_ << ", readFail_=" << readFail_ << std::endl;
                abort();
            }
            break;
        }
    }
    while (true) {
        Token nxtTkn(is);
        if (nxtTkn == Token::Fail) {
            readFail_ = true;
            break;
        }
        if (nxtTkn == Token::Eof) {
            if (eb == ebFailOnEof) {
                eofFail_ = true;
            }
            break;       
        }
        if (nxtTkn == Token::semiColon) {
            break;
        }
        push_back(nxtTkn);
    }
    readPos_ = 0;
}


void ds::VecToken::debugWrite(std::ostream& os) const {
    for (auto it = cbegin(); it != cend(); ++it) {
        os << " ";
        it->debugWrite(os);
    }
}


void ds::VecToken::errorWrite(std::ostream& os) const {
    auto it = cbegin();
    auto rp = readPos();
    if (rp != cbegin()) {
        --rp;
    }
    if (it != cend()) {
        if (it == rp) {
            os << "*" << *it << "*";
        } else {
            os << *it;
        }
        ++it;
    }
    for (; it != cend(); ++it) {
        if (it == rp) {
            os << "*" << *it << "*";
        } else {
            os << *it;
        }
    }
}


// ****** Template Specialisations ****** //

namespace ds
{

template<>
void ds::VecToken::assertGet(Token::typeEnum t) const {
    const Token& nextToken(get());
    if (nextToken == t) {
        return;
    }
    FatalError << "Expecting '" << Token::typeEnumToString(t) << "', got '" << nextToken << "'. "
        << "Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
    abort();
}


template<>
void ds::VecToken::assertGet(Token::punctuationEnum pe) const {
    const Token& nextToken(get());
    if (nextToken == pe) {
        return;
    }
    FatalError << "Expecting '" << Token::punctuationEnumToChar(pe) << "', got '" << nextToken
        << "'. Full token list is:\n";
        errorWrite(std::cerr);
        std::cerr << std::endl;
    abort();
}

} // end namespace


// ****** Global operators ****** //

std::ostream& ds::operator<<(std::ostream& os, const VecToken& vt) {
    auto it = vt.cbegin();
    if (it != vt.cend()) {
        os << *it;
        ++it;
    }
    for (; it != vt.cend(); ++it) {
        os << " " << *it;
    }
    return os;
}


// ****** END ****** //