#include <VecToken.h>

// ****** Constructors ****** //

ds::VecToken::VecToken():
    eofFail_(false),
    readFail_(false)
{}

VecToken::VecToken(std::istream& is):
    eofFail_(false),
    readFail_(false)
{
    read(is);
}


// ****** Public Member Functions ****** //

bool ds::VecToken::good() const {
    return !eofFail_ && !readFail_;
}


bool ds::VecToken::eofFail() const {
    return eofFail_
}


bool ds::VecToken::readFail() const {
    return readFail_;
}


void read(
    std::istream& is,
    bool setEofFail=true,
    readBehaviourEnum rb=rbFailIfNotEmpty
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
            return ret;
        }
        if (nxtTkn == Token::Eof) {
            if (setEofFail) {
                eofFail_ = true;
            }
            return ret;       
        }
        if (nxtTkn == Token::semiColon) {
            return ret;
        }
        ret.push_back(nxtTkn);
    }
}


void ds::VecToken::debugWrite(std::ostream& os) const {
    for (auto it = cbegin(); it != cend(); ++it) {
        os << " ";
        it->debugWrite(os);
    }
}


// ****** Global operators ****** //

std::ostream& ds::operator<<(std::ostream& os, const VecToken& vt) {
    it = vt.cbegin();
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