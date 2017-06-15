#include <cctype>
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
}


ds::Token::Token(char chr):
    type_(UnknownType),
    punctuation_(unknownPunctuation),
    integer_(0),
    uinteger_(0),
    double_(0)
{
    // Simple char read algorithm
    str_ += chr;
    punctuation_ = charToPunctuationEnum(chr);
    if (punctuation_ != unknownPunctuation)
    {
        type_ = Punctuation;
    } else if (validWordFirstChar(chr)) {
        type_ = Word;
    } else if (isdigit(chr)) {
        type_ = Float_Int_UInt;
        integer_ = std::strtoll(str_.c_str(), nullptr, 0);
        uinteger_ = integer_;
        double_ = integer_;
    } else {
        type_ = Fail;
    }
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


bool ds::Token::isBool() const {
    std::string tmp = str_;
    for (auto it = tmp.begin(); it != tmp.end(); ++it) {
        *it = std::toupper(*it);
    }
    return
        tmp == "1"
     || tmp == "0"
     || tmp == "Y"
     || tmp == "N"
     || tmp == "YES"
     || tmp == "NO"
     || tmp == "T"
     || tmp == "F"
     || tmp == "TRUE"
     || tmp == "FALSE"
     || tmp == "ON"
     || tmp == "OFF";
}

bool ds::Token::getBool() const {
    std::string tmp = str_;
    for (auto it = tmp.begin(); it != tmp.end(); ++it) {
        *it = std::toupper(*it);
    }
    if (
        tmp == "1"
     || tmp == "Y"
     || tmp == "YES"
     || tmp == "T"
     || tmp == "TRUE"
     || tmp == "ON"
    ) {
        return true;
    } else if (
        tmp == "0"
     || tmp == "N"
     || tmp == "NO"
     || tmp == "F"
     || tmp == "FALSE"
     || tmp == "OFF"
    ) {
        return false;
    } else {
        FatalError << "Expecting a bool (0,1,N,Y,YES,NO,T,F,TRUE,FALSE,ON,OFF). Got '" << str_
            << "'." << std::endl;
        abort();
    }
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


void ds::Token::debugWrite(std::ostream& os) const {
    os << "str='" << str_ << "', type='" << typeEnumToString(type_) << "', pType='"
        << punctuationEnumToChar(punctuation_) << "', int=" << integer_ << ", uint="
        << uinteger_ << ", dbl='" << double_ << "'  ";
}


// ****** Private Member Functions ****** //

bool ds::Token::read(std::istream& is) {
    if (!is.good()) {
        type_ = Fail;
        return false;
    }
    // Look for start of next token, avoid commented out sections
    // '/' could be 'divide', but it could also be a comment '//' or '/*'
    while (true) {
        int nxt(is.peek());
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
        char nextChar(nxt);
        punctuation_ = charToPunctuationEnum(nextChar);
        bool tryAgain = false;
        switch (punctuation_) {
            case divide: {
                // Check for comment
                is.get();
                int nxtnxt(is.peek());
                // Check for EOF
                if (nxtnxt == EOF) {
                    if (is.fail()) {
                        type_ = Fail;
                        return false;
                    } else {
                        type_ = Eof;
                        return true;
                    }
                }
                char nextCharNext(nxtnxt);
                punctuationEnum pe = charToPunctuationEnum(nextCharNext);
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
                    str_ += nextChar;
                    return true;
                }
            } // end case divide
            case subtract: // fall through
            case add: // fall through
            case period: {
                // Could be the start of a number, store this fact as type_ = Punctuation
                is.get();
                type_ = Punctuation;
                str_ += nextChar;
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
                str_ += nextChar;
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
        // Comments are cleared
        break;
    }

    // We are at the start of the next token
    //  * we have eliminated "string" tokens
    //  * we have eliminated most punctuation tokens, but may have a punctuation +-. at the front
    // First, check and get rid of front punctuation
    int nxt(is.peek());
    if (nxt == EOF) {
        // Can only be a +-. punctuation, everything is set
        return true;
    }
    char nextChar(nxt);

    if (type_ == Punctuation) {
        if (std::isspace(nxt) || validWordFirstChar(nextChar)) {
            // Can only be a +-. punctuation, everything is set
            return true;
        } else {
            // Temporarily use 'Float' type to indicate this is a number, not a word
            type_ = Float;
        }
    } else {
        if (validWordFirstChar(nextChar)) {
            type_ = Word;
        } else if (std::isdigit(nextChar)) {
            // Temporarily use 'Float' type to indicate this is a number, not a word
            type_ = Float;
        } else {
            str_ += nextChar;
            FatalError << "Disallowed character in input stream. Character is code " << nxt
                << ". Read: '" << str_ << "'" << std::endl;
            abort();
        }
    }

    str_ += nextChar;
    is.get();

    // Now we have either a word or a number
    if (type_ == Word) {
        getFullWord(is);
        return true;
    }
    #ifdef DSDEBUG
    if(type_ != Float) {
        FatalError << "Internal error - type_ should either be Float or Word at this point."
            << std::endl;
        abort();
    }
    #endif
    getFullNumber(is);
    return true;
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


void ds::Token::getFullWord(std::istream& is) {
    // First character is already taken, type is already Word
    while (true) {
        int nxt(is.peek());
        if (nxt == EOF || std::isspace(nxt)) {
            break;
        }
        char nextChar(nxt);
        if (validWordNthChar(nextChar)) {
            is.get();
            str_ += nextChar;
        } else {
            break;
        }
    }
}


void ds::Token::getFullNumber(std::istream& is) {
    // punctuation_ holds any preceding '+-.' or unknownPunctuation if none exist
    // First numerical character is already taken
    // type_ is set to Float, but needs to be changed as necessary

    //- True if number already has a decimal
    bool hasDecimal = punctuation_ == period;
    //- True when 'e' or 'E' is encountered, but only until the next character
    bool expStarted = false;
    //- True when expStarted and '+' or '-' are encountered, next character must be digit
    bool expSigned = false;
    //- Exponent pattern complete, cannot encounter again
    bool expCompleted = false;

    while (true) {
        int nxt(is.peek());
        if (nxt == EOF || std::isspace(nxt)) {
            break;
        }
        char nextChar(nxt);
        if (nextChar == 'e' || nextChar == 'E') {
            str_ += nextChar;
            if (expStarted || expCompleted) {
                FatalError << "Unrecognized exponent pattern: '" << str_ << "'" << std::endl;
                abort();
            }
            expStarted = true;
            is.get();
            continue;
        }
        if (nextChar == '+' || nextChar == '-') {
            str_ += nextChar;
            if (!expStarted || expSigned || expCompleted) {
                FatalError << "Unexpected '+' or '-' in number. Read: '" << str_ << "'"
                    << std::endl;
                abort();
            }
            expSigned = true;
            is.get();
            continue;
        }
        if (nextChar == '.') {
            str_ += nextChar;
            if (expStarted || expCompleted) {
                FatalError  << "Decimal encountered after exponent in number. Read: '" << str_
                    << std::endl;
                abort();
            }
            if (hasDecimal) {
                FatalError << "Decimal encountered twice in number. Read: '" << str_ << std::endl;
                abort();
            }
            hasDecimal = true;
            is.get();
            continue;
        }
        if (std::isdigit(nxt)) {
            if (expStarted) {
                expCompleted = true;
                expStarted = false;
                expSigned = false;
            }
            str_ += nextChar;
            is.get();
            continue;
        }
        // nextChar is not number-related
        break;
    }

    // Parse number
    if (expStarted) {
            FatalError << "Unexpected exponent pattern. Read: '" << str_ << "'" << std::endl;
            abort();
    }
    if (expCompleted) {
        // double_ is the basis of comparison
        double_ = std::strtod(str_.c_str(), nullptr);
        integer_ = double_;
        if (punctuation_ == subtract) {
            // Negative number only
            if (integer_ == double_) {
                type_ = Float_Int;
                return;
            } else {
                integer_ = 0;
                type_ = Float;
                return;
            }
        } else {
            uinteger_ = double_;
            if (integer_ == double_) {
                type_ = Float_Int_UInt;
                return;
            } else if (uinteger_ == double_) {
                type_ = Float_UInt;
                integer_ = 0;
                return;
            } else {
                type_ = Float;
                integer_ = 0;
                uinteger_ = 0;
                return;
            }
        }
    }
    if (punctuation_ != subtract) {
        // uinteger_ is the basis of comparison
        uinteger_ = std::strtoull(str_.c_str(), nullptr, 0);
        integer_ = uinteger_;
        double_ = uinteger_;
        if (uinteger_ == integer_) {
            if (uinteger_ == double_) {
                type_ = Float_Int_UInt;
                return;
            } else {
                double_ = 0.;
                type_ = Int_UInt;
                return;
            }
        } else {
            integer_ = 0;
            if (uinteger_ == double_) {
                type_ = Float_UInt;
                return;
            } else {
                double_ = 0;
                type_ = UInt;
                return;
            }
        }
    } else {
        // integer_ is the basis of comparison
        integer_ = std::strtoll(str_.c_str(), nullptr, 0);
        double_ = integer_;
        if (integer_ == double_) {
            type_ = Float_Int;
            return;
        }
        else {
            integer_ = 0;
            type_ = Float;
            return;
        }
    }
}


bool ds::Token::validWordFirstChar(char c) {
    return (std::isalpha(c) || c == '_');
}


bool ds::Token::validWordNthChar(char c) {
    return (std::isalpha(c) || c == '_' || std::isdigit(c));
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


bool ds::operator==(const Token& t, char c) {
    return t.str().at(0) == c;
}


bool ds::operator==(char c, const Token& t) {
    return t.str().at(0) == c;
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