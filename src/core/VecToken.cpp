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