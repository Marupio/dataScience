#include <Token.h>

const ds::Token ds::Token::eofToken;

ds::Token::Token():
    type_(Eof),
    punctuation_(unknownPunctuation),
    integer_(0),
    uinteger_(0),
    double_(0)
{}


ds::Token::Token(std::istream& is):
    type_(UnknownType),
    punctuation_(unknownPunctuation),
    integer_(0),
    uinteger_(0),
    double_(0)
{
    read(is);
    // if (!read(is)) {
    //     FatalError << "Token read failure. Got '" << str_ "'" << std::endl;
    //     abort();
    // }
}


// ****** Public Member Functions ****** //

ds::Token::typeEnum ds::Token::type() const {
    return type_;
}


ds::Token::punctuationEnum ds::Token::punctuation() const {
    return punctuation_;
}


std::string ds::Token::str() const {
    return str_;
}


std::string ds::Token::typeEnumToString(const typeEnum& te) {
    switch (te) {
        case UnknownType: return "UnknownType";
        case Fail: return "Fail";
        case Eof: return "Eof";
        case Punctuation: return "Punctuation";
        case Word: return "Word";
        case String: return "String";
        case Float: return "Float";
        case Int: return "Int";
        case UInt: return "UInt";
        case Float_Int: return "Float_Int";
        case Float_UInt: return "Float_UInt";
        case Int_UInt: return "Int_UInt";
        case Float_Int_UInt: return "Float_Int_UInt";
        default:
            FatalError << "Unknown typeEnum " << te << std::endl;
            abort();
    } // end switch
}


ds::Token::punctuationEnum ds::Token::charToPunctuationEnum(const char& c) {
    switch (c) {
        case 40: return openBracket;
        case 41: return closeBracket;
        case 123: return openScope;
        case 125: return closeScope;
        case 91: return openBlock;
        case 93: return closeBlock;
        case 60: return openAngle;
        case 62: return closeAngle;
        case 34: return openString;
        //case 34: return closeString;
        case 59: return semiColon;
        case 58: return colon;
        case 44: return comma;
        case 46: return period;
        case 47: return divide;
        case 42: return multiply;
        case 43: return add;
        case 45: return subtract;
        case 61: return equals;
        // case 62: return greaterThan;
        // case 60: return lessThan;
        case 124: return bar;
        case 38: return ampersand;
        case 33: return exclamation;
        case 64: return at;
        case 35: return pound;
        case 37: return percent;
        default: return unknownPunctuation;
    }
}


char ds::Token::punctuationEnumToChar(punctuationEnum pe) {
    if (pe == unknownPunctuation) {
    }
    switch(pe) {
        case unknownPunctuation: return '?';
        case openBracket: return '(';
        case closeBracket: return ')';
        case openScope: return '{';
        case closeScope: return '}';
        case openBlock: return '[';
        case closeBlock: return ']';
        case closeAngle: return '>';
        case openAngle: return '<';
        case openString: return '"';
        //case closeString: return '"';
        case semiColon: return ';';
        case colon: return ':';
        case comma: return ',';
        case period: return '.';
        case divide: return '/';
        case multiply: return '*';
        case add: return '+';
        case subtract: return '-';
        case equals: return '=';
        // case greaterThan: return '>';
        // case lessThan: return '<';
        case bar: return '|';
        case ampersand: return '&';
        case exclamation: return '!';
        case at: return '@';
        case pound: return '#';
        case percent: return '%';
        default: {
            FatalError << "Unknown punctuation type" << std::endl;
            abort();
        }
    }
}


size_t ds::Token::size() const {
    return str_.size();
}


void ds::Token::putBack(std::istream& is) {
    size_t count = str_.size();
    while (count--) {
        is.unget();
        if (is.fail()) {
            FatalError << "Failed to putBack '" << str_ << "' after character "
                << (str_.size() - count) << " from the right." << std::endl;
            abort();
        }
    }
}


bool ds::Token::isNotGood() const {
    return type_ == Fail;
}


bool ds::Token::isEof() const {
    return type_ == Eof;
}


bool ds::Token::isPunctuation() const {
    return type_ == Punctuation;
}


ds::Token::punctuationEnum ds::Token::getPunctuation() const {
    return punctuation_;
}


bool ds::Token::isWord() const {
    return type_ == Word;
}


std::string ds::Token::getWord() const {
    #ifdef DSDEBUG
    if (!isWord()) {
        FatalError << "Token is not a word." << std::endl;
        abort();
    }
    #endif
    return str_;
}


bool ds::Token::isString() const {
    return type_ == String;
}


std::string ds::Token::getString() const {
    #ifdef DSDEBUG
    if (!isString()) {
        FatalError << "Token is not a word." << std::endl;
        abort();
    }
    #endif
    return str_;
}


bool ds::Token::isInt() const {
    return
        type_ == Int
     || type_ == UInt
     || type_ == Float_Int
     || type_ == Float_UInt
     || type_ == Int_UInt
     || type_ == Float_Int_UInt;
}


short ds::Token::getShort() const {
    #ifdef DSDEBUG
    if (!isInt()) {
         FatalError << "Token is not an Int" << std::endl;
         abort();
    }
    #endif
    short ret = (short)integer_;
    if (ret != integer_) {
        Warning << "Overflow from input value '" << integer_ << "' casting to '" << ret << "'"
            << std::endl;
        abort();
    }
    return ret;
}


int ds::Token::getInt() const {
    #ifdef DSDEBUG
    if (!isInt()) {
         FatalError << "Token is not an Int" << std::endl;
         abort();
    }
    #endif
    int ret = (int)integer_;
    if (ret != integer_) {
        Warning << "Overflow from input value '" << integer_ << "' casting to '" << ret << "'"
            << std::endl;
        abort();
    }
    return ret;
}


long ds::Token::getLong() const {
    #ifdef DSDEBUG
    if (!isInt()) {
         FatalError << "Token is not an Int" << std::endl;
         abort();
    }
    #endif
    long ret = (long)integer_;
    if (ret != integer_) {
        Warning << "Overflow from input value '" << integer_ << "' casting to '" << ret << "'"
            << std::endl;
        abort();
    }
    return ret;
}


long long ds::Token::getLongLong() const {
    #ifdef DSDEBUG
    if (!isInt()) {
         FatalError << "Token is not an Int" << std::endl;
         abort();
    }
    #endif
    return integer_;
}


bool ds::Token::isUInt() const {
    return
        type_ == UInt
     || type_ == Float_UInt
     || type_ == Int_UInt
     || type_ == Float_Int_UInt;
}


unsigned short ds::Token::getUShort() const {
    #ifdef DSDEBUG
    if (!isUInt()) {
         FatalError << "Token is not a UInt" << std::endl;
         abort();
    }
    #endif
    unsigned short ret = (unsigned short)uinteger_;
    if (ret != uinteger_) {
        Warning << "Overflow from input value '" << uinteger_ << "' casting to '" << ret << "'"
            << std::endl;
        abort();
    }
    return ret;
}


unsigned int ds::Token::getUInt() const {
    #ifdef DSDEBUG
    if (!isUInt()) {
         FatalError << "Token is not a UInt" << std::endl;
         abort();
    }
    #endif
    unsigned int ret = (unsigned int)uinteger_;
    if (ret != uinteger_) {
        Warning << "Overflow from input value '" << uinteger_ << "' casting to '" << ret << "'"
            << std::endl;
        abort();
    }
    return ret;
}


unsigned long ds::Token::getULong() const {
    #ifdef DSDEBUG
    if (!isUInt()) {
         FatalError << "Token is not a UInt" << std::endl;
         abort();
    }
    #endif
    unsigned long ret = (unsigned long)uinteger_;
    if (ret != uinteger_) {
        Warning << "Overflow from input value '" << uinteger_ << "' casting to '" << ret << "'"
            << std::endl;
        abort();
    }
    return ret;
}


unsigned long long ds::Token::getULongLong() const {
    #ifdef DSDEBUG
    if (!isUInt()) {
         FatalError << "Token is not a UInt" << std::endl;
         abort();
    }
    #endif
    return uinteger_;
}


bool ds::Token::isFloat() const {
    return
        type_ == Float
     || type_ == Float_Int
     || type_ == Float_UInt
     || type_ == Float_Int_UInt;
}


float ds::Token::getFloat() const {
    #ifdef DSDEBUG
    if (!isFloat()) {
         FatalError << "Token is not a Float" << std::endl;
         abort();
    }
    #endif
    float ret = (float)double_;
    if (ret != double_) {
        Warning << "Overflow from input value '" << double_ << "' casting to '" << ret << "'"
            << std::endl;
        abort();
    }
    return ret;
}


double ds::Token::getDouble() const {
    #ifdef DSDEBUG
    if (!isFloat()) {
         FatalError << "Token is not a Float" << std::endl;
         abort();
    }
    #endif
    return double_;
}


// ****** Private Member Functions ****** //

bool ds::Token::read(std::istream& is) {
    if (!is.good()) {
        type_ = Fail;
        return false;
    }
    // Look for start of next token, avoid commented out sections
    // '/'' could be 'divide', but it could also be a comment '//'' or '/*''
    while (true) {
        int nxt(is.peek());
        char charNext(nxt);
        punctuation_ = charToPunctuationEnum(charNext);
        bool tryAgain = false;
        switch (punctuation_) {
            case divide: {
                // Check for comment
                is.get();
                int nxtnxt(is.peek());
                char charNextNext(nxtnxt);
                punctuationEnum pe = charToPunctuationEnum(charNextNext);
                if (pe == divide) {
                    // Line comment
                    is.get();
                    type_ = UnknownType;
                    punctuation_ = unknownPunctuation;
                    if (!movePastEndOfLine(is)) {
                        type_ = Fail;
                        return false;
                    }
                    tryAgain = true;
                } else if (pe == multiply) {
                    // Block comment
                    is.get();
                    type_ = UnknownType;
                    punctuation_ = unknownPunctuation;
                    moveToEndOfBlockComment(is);
                    tryAgain = true;
                } else {
                    // Really is a divide punctuation
                    type_ = Punctuation;
                    str_ = punctuationEnumToChar(divide);
                    return true;
                }
            } // end case divide
            case subtract: // fall through
            case add: // fall through
            case period: {
                // Could be the start of a number, store this fact as type_ = Punctuation
                is.get();
                type_ = Punctuation;
                str_ = punctuationEnumToChar(divide);
                break;
            }
            case unknownPunctuation: {
                // Not a punctuation
                break;
            }
            default: {
                // Punctuation token
                is.get();
                type_ = Punctuation;
                str_ = punctuationEnumToChar(divide);
                return true;
            }
        }
        if (tryAgain) {
            continue;
        }
        if (type_ == Punctuation) {
            break;
        }
        // Check for whitespace
        if (std::isspace(nxt)) {
            is.get();
            continue;
        }
        // Check for EOF
        if (nxt == EOF) {
            if (is.fail()) {
                type_ = Fail;
                return false;
            } else {
                type_ = Eof;
                return true;
            }
        }
        // Comments are cleared
        break;
    }

    // We are at the start of the next token, but may have a punctuation +-. at the front
    int nxt(is.peek());
    char charNext(nxt);

    // if space or eof, send punctuation
    // if alpha set word
    // if number set number
    // then go on to next bunch    

        if (type_ == Punctuation) {
            if (charNext == punctuationEnumToChar(divide)) {
                is.get();
                type_ = UnknownType;
                punctuation_ = unknownPunctuation;
                if (!movePastEndOfLine(is)) {
                    type_ = Fail;
                    return false;
                }
            } else if (charNext == punctuationEnumToChar(multiply)) {
                is.get();
                type_ = UnknownType;
                punctuation_ = unknownPunctuation;
                moveToEndOfBlockComment(is);
                continue;
            } else {
                // It is a divide punctuation
                str_ = punctuationEnumToChar(divide);
                return true;
            }
        }
        if (std::isspace(nxt)) {
            is.get();
            continue;
        }
        if (nxt == EOF) {
            if (is.fail()) {
                type_ = Fail;
                return false;
            } else {
                type_ = Eof;
                return true;
            }
        }
        if (charNext == punctuationEnumToChar(divide)) {
            type_ = Punctuation;
            punctuation_ = divide;
            is.get();
            continue;
        }
        break;
    }

    // We are at the start of the next token
    // We may be holding a punctuation +-. in case it's a number
    char firstChar;
    is.get(firstChar);
    str_ += firstChar;
    punctuation_ = charToPunctuationEnum(firstChar);
    switch(punctuation_) {
        case openString: {
            getFullString(is);
            type_ = String;
            punctuation_ = unknownPunctuation;
            return true;
        }
        case unknownPunctuation: // fall through
        case period: // could be decimal number: fall through
        case subtract: // could be negative number: fall through
        case add: // could be explicitly positive number: fall through
            break;
        default: {
            type_ = Punctuation;
            return true;
        }
    }

    bool numEncountered = false;
    bool periodEncountered = punctuation_ == period;
    bool exponentStarted = false;
    bool exponentSigned = false;
    bool exponentCompleted = false;
    while (true) {
        // Edge cases:
        // * '-' could be 'subtract', but it could also be a number
        // * '0'-'9' could be a number, but it could also be a word starting with a number
        // * 'e' or 'E' could be exponentiation
        // * '.' could be decimal, or a separator
        int nxt = is.peek();
        if (is.fail()) {
            type_ = Fail;
            return false;
        }

        // Ending a token:
        // nxt == EOF, or isspace
        // Type changes:
        //  * Word to any punctuation
        //  * any punctuation to alpha (handled)
        //  * any punctuation except +-. to number
        //  * number to any punctuation


        if (nxt == EOF || std::isspace(nxt)) {
            // End of token
            if (exponentSigned || (exponentStarted && periodEncountered)) {
                // Error - incomplete exponent, or word with punctuation
            }
            if (exponentStarted) {
                // Just a number-started word ending with e
                type_ = Word;
                return true;
            }
            if (type_ == Word) {
                return true;
            }
            if (numEncountered) {
                if (exponentCompleted) {
                    // double only
                    double_ = std::stod(str_);
                    type_ = Float;
                    return true;
                }
                if (punctuation_ != subtract) {
                    // uinteger_ is the basis of comparison
                    uinteger_ = std::stoull(str_);
                    integer_ = std::stoll(str_);
                    double_ = std::stod(str_);
                    if (uinteger_ == integer_) {
                        if (uinteger_ == double_) {
                            type_ = Float_Int_UInt;
                            return true;
                        } else {
                            double_ = 0.;
                            type_ = Int_UInt;
                            return true;
                        }
                    } else {
                        if (uinteger_ == double_) {
                            type_ = Float_UInt;
                            return true;
                        } else {
                            integer_ = 0.;
                            type_ = UInt;
                            return true;
                        }
                    }
                } else {
                    // integer_ is the basis of comparison
                    integer_ = std::stoll(str_);
                    double_ = std::stod(str_);
                    if (integer_ == double_) {
                        type_ = Float_Int;
                        return true;
                    }
                    else {
                        integer_ = 0;
                        type_ = Float;
                        return true;
                    }
                }
            }
            if (punctuation_ != unknownPunctuation) {
                type_ = Punctuation;
                return true;
            }
            // Should have captured all types of tokens by now, internal error if reached here
            type_ = UnknownType;
            return false;
        }
        char nextChar;
        is.get(nextChar);
        str_ += nextChar;

        if (exponentStarted) {
            if (std::isdigit(nextChar)) {
                exponentStarted = false;
                exponentSigned = false;
                exponentCompleted = true;
                continue;
            }
            if (
                nextChar == punctuationEnumToChar(subtract)
             || nextChar == punctuationEnumToChar(add)
            ) {
                if (exponentSigned) {
                    FatalError << "Unexpected exponent pattern: '" << str_ << "'" << std::endl;
                    abort();
                } else {
                    exponentSigned = true;
                    continue;
                }
            }
        }

        if (std::isdigit(nextChar)) {
            numEncountered = true;
            continue;
        }
        if (nextChar == 'e' || nextChar == 'E') {
            if (type_ == Word || !numEncountered) {
                continue;
            }
            exponentStarted = true;
            continue;
        }

        if (nextChar == punctuationEnumToChar(period)) {
            if (periodEncountered) {
                FatalError << "Unexpected number pattern: '" << str_ << "'" << std::endl;
                abort();
            } else if (type_ == Word) {
                FatalError << "Puntuation not expected here: '" << str_ << "'" << std::endl;
                abort();
            } else {
                periodEncountered = true;
            }
        }
        if (std::isalpha(nextChar) || nextChar == '_') {
            if (
                periodEncountered
             || punctuation_ == subtract
             || punctuation_ == add
             || exponentSigned
            ) {
                FatalError << "Unexpected word pattern '" << str_ << "'" << std::endl;
                abort();
            }
            type_ = Word;
            continue;
        }
        if (charToPunctuationEnum(nextChar) != unknownPunctuation) {
            FatalError << "Punctuation not expected here: '" << str_ << "'" << std::endl;
            abort();
        } else {
            FatalError << "Disallowed character in input stream: '" << str_ << "'" << std::endl;
            abort();
        }
    }
}


bool ds::Token::movePastEndOfLine(std::istream& is) {
    while (true) {
        int nxt = is.peek();
        if (nxt == EOF) {
            if (is.fail()) {
                return false;
            }
            return true;
        }
        if (char(nxt) == '\n') {
            is.get();
            return true;
        }
        is.get();
    }
}


void ds::Token::moveToEndOfBlockComment(std::istream& is) {
    while (true) {
        int nxt = is.peek();
        if (nxt == EOF) {
            FatalError << "Could not find end of comment block." << std::endl;
            abort();
        }
        if (char(nxt) == punctuationEnumToChar(multiply)) {
            is.get();
            int nxtnxt = is.peek();
            if (nxtnxt == EOF) {
                FatalError << "Could not find end of comment block." << std::endl;
                abort();
            }
            if (char(nxtnxt) == punctuationEnumToChar(divide)) {
                is.get();
                return;
            }
            continue;
        }
        is.get();
    }
}


void ds::Token::getFullString(std::istream& is) {
    while (true) {
        int nxt = is.peek();
        char nextChar(nxt);
        if (nxt == EOF) {
            FatalError << "Could not find end of string." << std::endl;
            abort();
        }
        if (nextChar == punctuationEnumToChar(closeString)) {
            str_ += nextChar;
            is.get();
            return;
        }
        // TODO - Handle line breaks
        str_ += nextChar;
        is.get();
    }
}


void ds::Token::debugWrite(std::ostream& os) const {
    os << "str='" << str_ << "', type='" << typeEnumToString(type_) << "', pType='"
        << punctuationEnumToChar(punctuation_) << ", int=" << integer_ << ", uint="
        << uinteger_ << ", dbl='" << double_ << "'  ";
}


// ****** Global operators ****** //

std::ostream& ds::operator<<(std::ostream& os, const Token& t) {
    os << t.str_;
    return os;
}


std::ostream& ds::operator<<(std::ostream& os, const Token::punctuationEnum& tpe) {
    os << Token::punctuationEnumToChar(tpe);
    return os;
}


bool ds::operator==(const Token& t, Token::typeEnum te) {
    return t.type() == te;
}


bool ds::operator==(Token::typeEnum te, const Token& t) {
    return t.type() == te;
}


bool ds::operator==(const Token& t, Token::punctuationEnum pe) {
    return t.punctuation() == pe;
}


bool ds::operator==(Token::punctuationEnum pe, const Token& t) {
    return t.punctuation() == pe;
}


bool ds::operator==(const Token& t, const std::string& str) {
    return str.compare(t.str()) == 0;
}


bool ds::operator==(const std::string& str, const Token& t) {
    return str.compare(t.str()) == 0;;
}


bool ds::operator==(const Token& t, unsigned short num) {
    return t.getUShort() == num;
}


bool ds::operator==(unsigned short num, const Token& t) {
    return t.getUShort() == num;
}


bool ds::operator==(const Token& t, short num) {
    return t.getShort() == num;
}


bool ds::operator==(short num, const Token& t) {
    return t.getShort() == num;
}


bool ds::operator==(const Token& t, unsigned int num) {
    return t.getUInt() == num;
}


bool ds::operator==(unsigned int num, const Token& t) {
    return t.getUInt() == num;
}


bool ds::operator==(const Token& t, int num) {
    return t.getInt() == num;
}


bool ds::operator==(int num, const Token& t) {
    return t.getInt() == num;
}


bool ds::operator==(const Token& t, unsigned long num) {
    return t.getULong() == num;
}


bool ds::operator==(unsigned long num, const Token& t) {
    return t.getULong() == num;
}


bool ds::operator==(const Token& t, long num) {
    return t.getLong() == num;
}


bool ds::operator==(long num, const Token& t) {
    return t.getLong() == num;
}


bool ds::operator==(const Token& t, unsigned long long num) {
    return t.getULongLong() == num;
}


bool ds::operator==(unsigned long long num, const Token& t) {
    return t.getULongLong() == num;
}


bool ds::operator==(const Token& t, long long num) {
    return t.getLongLong() == num;
}


bool ds::operator==(long long num, const Token& t) {
    return t.getLongLong() == num;
}


bool ds::operator==(const Token& t, float num) {
    return t.getFloat() == num;
}


bool ds::operator==(float num, const Token& t) {
    return t.getFloat() == num;
}


bool ds::operator==(const Token& t, double num) {
    return t.getDouble() == num;
}


bool ds::operator==(double num, const Token& t) {
    return t.getDouble() == num;
}


// ****** END ****** //