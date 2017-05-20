#ifndef Token_h
#define Token_h

namespace ds
{

class Token
{
public:

    // Public Data Types

    enum typeEnum {
        unknownType,
        notGood,
        eof,
        punctuation,
        word,
        string,
        integer,
        floatNumber
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

private:

    // Private Member Functions

        static punctuationEnum charToPunctuationEnum(const char& c);

        static char punctuationEnumToChar(punctuationEnum pe);

        // Read next token from istream
        bool read(std::istream& is);

        // Read through istream until end-of line is encountered, if EOF, does not get it
        void movePastEndOfLine(std::istream& is);

        //- Read through istream until */ encountered, error if not found
        void moveToEndOfBlockComment(std::istream& is);


    // Private Member Data

        typeEnum type_;
        punctuationEnum punctuation_;
        size_t size_;
        std::string str_;
        long long integer_;
        unsigned long long uinteger_;
        double double_;
};

} // end namespace

#endif // Token_h
