#ifndef VecToken_h
#define VecToken_h

#include <Token.h>

namespace ds
{

class VecToken: public std::vector<Token> {
public:

    // Public Data Types

        //- Read behaviour
        enum readBehaviourEnum {
            rbFailIfNotEmpty,
            rbOverwrite,
            rbAppend
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
        void read(std::istream& is, readBehaviourEnum rb=rbFailIfNotEmpty);


private:

    // Private Member Data

        //- EOF encountered before ';'
        bool eofFail_;

        //- Read failure in istream
        bool readFail_;
};


} // end namespace

#endif // VecToken_h
