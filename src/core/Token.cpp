#include <Token.h>


Token(std::istream& is):
    type_(unknownType),
    punctuation_(unknownPunctuation),
    size_(0),
    integer_(0),
    uinteger_(0),
    double_(0)
{
    read(is);
}


    // Public Member Functions

        // Tests and value access

            //- Rewinds is to the position just before this token
            void putBack(std::istream& is);

            //- True if there was a problem reading from istream
            bool isNotGood() const;

            //- True if EOF was encountered before any token
            bool isEof() const;

            //- True if is punctuation
            bool isPunctuation() const;
            punctuationEnum getPunctuation() const;

            //- True if is a stand-alone set of alpha-numeric characters with no whitespace
            bool isWord() const;
            std::string getWord() const;

            //- True if is enclosed in string punctuation
            bool isString() const;
            std::string getString() const;

            //- True if is a number with no fractional component (exponents are okay)
            bool isInteger() const;
            unsigned short getUShort() const;
            short getShort() const;
            unsigned int getUInt() const;
            int getInt() const;
            unsigned long getULong() const;
            long getLong() const;
            unsigned long long getULongLong() const;
            long long getLongLong() const;

            //- True if is a number
            bool isFloat() const;
            float getFloat() const;
            double getDouble() const;


    // Public Member Operators

        //- Equality
        bool operator==(typeEnum& te);
        bool operator==(puncuationEnum& pe);
        bool operator==(std::string& str);
        bool operator==(unsigned short num);
        bool operator==(short num);
        bool operator==(unsigned int num);
        bool operator==(int num);
        bool operator==(unsigned long num);
        bool operator==(long num);
        bool operator==(unsigned long long num);
        bool operator==(long long num);
        bool operator==(float num);
        bool operator==(double num);


ds::Token::punctuationEnum ds::Token::charToPunctuationEnum(const char& c) {
    switch (c): {
        case 40: return openParenthesis;
        case 41: return closeParenthesis;
        case 123: return openScope;
        case 125: return closeScope;
        case 91: return openBlock;
        case 93: return closeBlock;
        case 34: return openString;
        case 34: return closeString;
        case 59: return semiColon;
        case 58: return colon;
        case 44: return comma;
        case 46: return period;
        case 47: return divide;
        case 42: return multiply;
        case 43: return add;
        case 45: return subtract;
        case 61: return equals;
        case 62: return greaterThan;
        case 60: return lessThan;
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
        case openParenthesis: return '(';
        case closeParenthesis: return ')';
        case openScope: return '{';
        case closeScope: return '}';
        case openBlock: return '[';
        case closeBlock: return ']';
        case openString: return '"';
        case closeString: return '"';
        case semiColon: return ';';
        case colon: return ':';
        case comma: return ',';
        case period: return '.';
        case divide: return '/';
        case multiply: return '*';
        case add: return '+';
        case subtract: return '-';
        case equals: return '=';
        case greaterThan: return '>';
        case lessThan: return '<';
        case bar: return '|';
        case ampersand: return '&';
        case exclamation: return '!';
        case at: return '@';
        case pound: return '#';
        case percent: return '%';
        default: {
            FatalError << "Attempting to get character of unknownPunctuation" << std::endl;
            abort();
        }
    }
}


bool ds::Token::read(std::istream& is) {
    if (!is.good()) {
        return false;
    }
    // Look for start of next token, avoid commented out sections
    // '/'' could be 'divide', but it could also be a comment '//'' or '/*''
    while (true) {
        int nxt(is.peek());
        char charNext(nxt);
        if (type_ == punctuation) {
            if (charNext == punctuationEnumToChar(divide)) {
                is.get();
                type_ = unknownType;
                punctuation_ = unknownPunctuation;
                movePastEndOfLine(is);
            } else if (charNext == punctuationEnumToChar(multiply)) {
                is.get();
                type_ = unknownType;
                punctuation_ = unknownPunctuation;
                moveToEndOfBlockComment(is);
            } else {
                // It is a divide punctuation
                return true;
            }
        }
        if (std::isspace(nxt)) {
            is.get();
            continue;
        }
        if (nxt == EOF) {
            type_ = eof;
            return true;
        }
        if (charNext == punctuationEnumToChar(divide)) {
            type_ = punctuation;
            punctuation_ = divide;
            is.get();
            continue;
        }
    }

    // We are at the start of the next token
    char firstChar;
    is.get(firstChar);
    punctuation_ = charToPunctuationEnum(firstChar);
    switch(punctuation_) {
        case openString: {
            getFullString(is);
            type_ = string;
            punctuation_ = unknownPunctuation;
            return true;
        }
        case unknownPunctuation: // fall through
        case period: // could be decimal number: fall through
        case subtract: // could be negative number: fall through
        case add: // could be explicitly positive number: fall through
            break;
        default: {
            type_ = punctuation;
            return true;
        }
    }

    bool numEncountered = false;
    bool periodEncountered = puncuation_ == period;
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
        if (nxt == EOF || std::isspace(nxt)) {
            // End of token
            if (type_ == word) {
                return true;
            }
            if (numEncountered) {
                if (punctuation_ != subtract) {
                    uinteger_ = std::stoull(str_);
                }
                integer_ = std::stoll(str_);
                double_ = std::strtod(str_);
                // Detect if fractional part exists
            }
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

        if (std::isdigit(nextChar)) {
            numEncountered = true;
            continue;
        }
        if (nextChar == 'e' || nextChar == 'E') {
            if (type_ == word || !numEncountered) {
                continue;
            }
            exponentStarted = true;
            continue;
        }

        if (nextChar == punctuationEnumToChar(period)) {
            if (periodEncountered) {
                FatalError << "Unexpected number pattern: '" << str_ << "'" << std::endl;
                abort();
            } else if (type_ == word) {
                FatalError << "Puntuation not expected here: '" << str_ << "'" << std::endl;
                abort();
            } else {
                periodEncountered = true;
            }
        }
        if (std::isalpha(nextChar) || nextChar == "_") {
            if (
                periodEncountered
             || punctuation_ == subtract
             || punctuation_ == add
             || exponentSigned
            ) {
                FatalError << "Unexpected word pattern '" << str_ << "'" << std::endl;
                abort();
            }
            type_ = word;
            continue;
        } if (charToPunctuationEnum(nextChar) != unknownPunctuation) {
            FatalError << "Punctuation not expected here: '" << str_ << "'" << std::endl;
            abort();
        } else {
            FatalError << "Disallowed character in input stream: '" << str_ << "'" << std::endl;
            abort();
        }
    }
}


void ds::Token::movePastEndOfLine(std::istream& is) {
    while (true) {
        int nxt = is.peek();
        if (nxt == EOF) {
            return;
        }
        if (char(nxt) == '\n') {
            is.get();
            return;
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
            is.get();
            return;
        }
        // TODO - Handle line breaks
        str_ += nextChar;
        is.get();
    }
}


// ****** END ****** //