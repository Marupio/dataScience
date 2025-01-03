#ifndef VecToken_h
#define VecToken_h

#include <Token.h>
#include <vector.h>

namespace ds
{

// Forward declarations

class VecToken;
std::ostream& operator<<(std::ostream& os, const VecToken& c);


class VecToken: public std::vector<Token> {
public:

    // Public Data Types

        //- Read behaviour
        enum readBehaviourEnum {
            rbFailIfNotEmpty,
            rbOverwrite,
            rbAppend
        };

        //- EOF fail behaviour
        enum eofBehaviourEnum {
            ebFailOnEof,
            ebAllowEof
        };

    // Constructors

        //- Construct null
        VecToken();

        //- Construct from istream
        VecToken(std::istream& is);


    // Public Member Functions

        //- Access
        bool good() const;
        bool eofFail() const;
        bool readFail() const;

        //- Returns readPos iterator
        std::vector<Token>::const_iterator readPos() const;

        //- Returns next token, but does not increment
        const Token& peek() const;

        //- Get next token, increment read position
        //  Returns Eof token when at end
        const Token& get() const;

        //- Assert that the size is the given size
        void assertSize(size_t n) const;

        //- Get the next token and ensure it isPunctuation
        Token::punctuationEnum assertGetPunctuation() const;

        //- Get the next token and ensure it isBool
        bool assertGetBool() const;

        //- Get the next token and ensure it isWord
        std::string assertGetWord() const;

        //- Get the next token and ensure it isString
        std::string assertGetString() const;

        //- Get the next token and ensure it isInt
        short assertGetShort() const;
        int assertGetInt() const;
        long assertGetLong() const;
        long long assertGetLongLong() const;

        //- Get the next token and ensure it isUInt
        unsigned short assertGetUShort() const;
        unsigned int assertGetUInt() const;
        unsigned long assertGetULong() const;
        unsigned long long assertGetULongLong() const;

        //- Get the next token and ensure it isFloat
        float assertGetFloat() const;
        double assertGetDouble() const;

        //- Get next token, and ensure it equals t
        template<class T>
        void assertGet(T t) const;

        //- Attempts to decrement read position by one, fails if already at start
        void unget() const;

        //- Moves read position to beginning
        void rewind() const;

        //- Read from istream
        void read(
            std::istream& is,
            eofBehaviourEnum eb=ebFailOnEof,
            readBehaviourEnum rb=rbFailIfNotEmpty
        );


    // Debug

        //- Debug write
        void debugWrite(std::ostream& os) const;

        //- Error write - writes out token list, putting * * around readPos
        void errorWrite(std::ostream& os) const;

    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const VecToken& vt);


private:

    // Private Member Data

        //- EOF encountered before ';'
        bool eofFail_;

        //- Read failure in istream
        bool readFail_;

        //- Current read position in VecToken, stored as an index to avoid iterator invalidation
        mutable size_t readPos_;
};


// Template specialisations

template<>
void VecToken::assertGet(Token::typeEnum& t) const;

template<>
void VecToken::assertGet(Token::punctuationEnum& pe) const;

} // end namespace

#include <VecTokenTemplates.cpp>

#endif // VecToken_h
