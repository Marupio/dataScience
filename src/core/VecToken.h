#ifndef VecToken_h
#define VecToken_h

#include <Token.h>

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

        //- Read from istream
        void read(
            std::istream& is,
            eofBehaviourEnum eb=ebFailOnEof,
            readBehaviourEnum rb=rbFailIfNotEmpty
        );

        //- Debug write
        void debugWrite(std::ostream& os) const;


    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const VecToken& vt);


private:

    // Private Member Data

        //- EOF encountered before ';'
        bool eofFail_;

        //- Read failure in istream
        bool readFail_;
};


} // end namespace

#endif // VecToken_h
