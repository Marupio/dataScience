#ifndef Itstream_h
#define Itstream_h

#include <Token.h>

class Itstream:
    public std::istream
{
public:

    // Constructors

        //- Construct from istream
        Itstream(std::istream& is);


    // Public Member Functions

        //- Access underlying istream
        istream& getStream();

        //- Rewind Itstream to beginning
        void rewind();

        //- Put token back into stream
        void putback(Token& t);

        //- Decrease the iterator position by one
        void ungetToken();



    // Public Member Operators

        Itstream& operator>>(bool& r);
        Itstream& operator>>(short& r);
        Itstream& operator>>(ushort& r);
        Itstream& operator>>(int& r);
        Itstream& operator>>(unsigned int& r);
        Itstream& operator>>(long& r);
        Itstream& operator>>(unsigned long& r);
        Itstream& operator>>(long long& r);
        Itstream& operator>>(unsigned long long& r);
        Itstream& operator>>(float& r);
        Itstream& operator>>(double& r);
        Itstream& operator>>(std::string& r);
        Itstream& operator>>(Token& r);
        Itstream& operator>>(Token::punctuationEnum& r);

private:

    //- Token list
    VecToken tokens_;

    //- Current position in token list
    VecToken::const_iterator iter_;
    
};


#endif // Itstream_h
