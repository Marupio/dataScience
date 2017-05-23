#ifndef Token_h
#define Token_h

#include <vector.h>

namespace ds
{

// Forward declarations

class Token;
std::ostream& operator<<(std::ostream& os, const Token& c);


class Token
{
public:

    // Public Data Types

    enum typeEnum {
        // Title case to avoid name collisions with actual types
        UnknownType,    // Not yet set, or internal error if read() already called, i.e. should not
                        // occur
        Fail,           // Problem reading from stream
        Eof,            // EOF encountered before next token
        Punctuation,    // Punctuation contained in punctuationEnum
        Word,           // Single word consisting of alpha-numeric chars or '_', no whitespaces
        String,         // Any characters enclosed by quotes ""
        Float,          // Any number, allows exponentiation
                        //  regex = [+-]?([0-9]*)?\.?([eE][+-]?)?[0-9]*
                        //  i.e. 12, -12.34, +12.34e7, -12.34E-300, 12e34
        Int,            // Any string of numbers, allows preceding sign, no exponentiation allowed
                        //  regex = [+-]?[0-9]*
                        //  i.e. 12, -34, +4600
        UInt,           // Any string of numbers, allows preceding '+', no exponentiation allowed
        Float_Int,      // Could be Float or Int, but not UInt
                        // This happens for negative numbers
        Float_UInt,     // Could be Float or UInt, but not Int
                        // This happens if the UInt is larger than Int max
        Int_UInt,       // Could be Int or UInt, but not Float
                        // This happens when Int precision exceeds that of Float
        Float_Int_UInt  // Could be any number type
    };

    enum punctuationEnum
    {
        unknownPunctuation,

        // enclosures
        openParenthesis,        // '('
        closeParenthesis,       // ')'
        openScope,              // '{'
        closeScope,             // '}'
        openBlock,              // '['
        closeBlock,             // ']'
        openString,             // '"'
        closeString = openString,

        // separators
        semiColon,              // ';'
        colon,                  // ':'
        comma,                  // ','
        period,                 // '.'

        // math
        divide,                 // '/'
        multiply,               // '*'
        add,                    // '+'
        subtract,               // '-'
        equals,                 // '='
        greaterThan,            // '>'
        lessThan,               // '<'

        // logic
        bar,                    // '|'
        ampersand,              // '&'
        exclamation,            // '!'

        // other
        at,                     // '@'
        pound,                  // '#'
        percent                 // '%'
    };


    // Constructors

        //- Construct from istream
        Token(std::istream& is);


    // Public Member Functions

        // Tests and value access

            //- Return number of characters of token
            size_t size() const;

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

            //- True if is an integer number
            bool isInt() const;
            short getShort() const;
            int getInt() const;
            long getLong() const;
            long long getLongLong() const;

            //- True if is a positive integer number
            bool isUInt() const;
            unsigned short getUShort() const;
            unsigned int getUInt() const;
            unsigned long getULong() const;
            unsigned long long getULongLong() const;

            //- True if is a number
            bool isFloat() const;
            float getFloat() const;
            double getDouble() const;


        // Debug

            //- Debug write
            void debugWrite(std::ostream& os) const;


    // Public Member Operators

        //- Equality
        bool operator==(typeEnum& te);
        bool operator==(punctuationEnum& pe);
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


    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const Token& t);


private:

    // Private Member Functions

        static std::string typeEnumToString(const typeEnum& te);

        static punctuationEnum charToPunctuationEnum(const char& c);

        static char punctuationEnumToChar(punctuationEnum pe);

        // Read next token from istream
        bool read(std::istream& is);

        // Read through istream until end-of line is encountered, if EOF, does not get it
        bool movePastEndOfLine(std::istream& is);

        //- Read through istream until */ encountered, error if not found
        void moveToEndOfBlockComment(std::istream& is);


    // Private Member Data

        //- Token type
        typeEnum type_;

        //- If is punctuation, contains punctuationType
        punctuationEnum punctuation_;

        std::string str_;
        long long integer_;
        unsigned long long uinteger_;
        double double_;
};


} // end namespace

#endif // Token_h
