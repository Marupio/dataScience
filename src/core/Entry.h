#ifndef Entry_h
#define Entry_h

#include <memory>
#include <Token.h>

namespace ds
{

class Entry {
public:

    // Constructors

        //- Construct from istream and parent dictionary
        Entry(Dictionary& parent, std::istream& is);


    // Public Member Functions

        // Access

            //- Access keyword
            std::string& keyword();
            const std::string& keyword() const;

            //- Access dictionary
            //  Not allocated if in stream entry mode
            Dictionary& dict();
            const Dictionary& dict() const;

            //- Access token stream
            //  Not valid if in dictionary mode
            VecToken& tokens();
            const VecToken& tokens() const;


        // Query

            //- True if type is a dictionary entry
            bool isDict() const;

            //- True if type is a token entry
            bool isTokenData() const;

            //- True if neither dictionary nor stream are allocated
            bool isNull() const;

private:

    // Private Member Functions

        //- Read keyword from istream
        //  Returns false if not valid, or not found
        bool getKeyword(std::istream& is);

        //- Read istream into Itstream
        void readStreamEntry(std::istream& is);


    // Private Member Data

        //- Associated keyword
        std::string keyword_;

        //- Dictionary data (dictionary mode)
        std::unique_ptr<Dictionary> dictPtr_;

        //- Token data (token mode)
        std::unique_ptr<VecToken> tokensPtr_;

        //- Parent dictionary
        const Dictionary& parent_;
    
};

} // end namespace

#endif // Entry_h
