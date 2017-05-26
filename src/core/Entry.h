#ifndef Entry_h
#define Entry_h

#include <memory>
#include <types.h>

namespace ds
{

// Forward declarations

class Dictionary;

class Entry;
std::ostream& operator<<(std::ostream& os, const Entry& c);

class Entry {
public:

    // Public Data Types

        //- Type of Entry
        enum typeEnum
        {
            teUnknown,
            teEof,
            teDict,
            teTokens
        };

    // Constructors

        //- Construct from istream and parent dictionary
        Entry(Dictionary& parent, std::istream& is, bool requireSemiColonEnd=true);

        //- Construct given keyword and istream
        Entry(
            Dictionary& parent,
            const std::string& keyword,
            std::istream& is,
            bool requireSemiColonEnd=true
        );

        // Construct from subdictionary
        Entry(Dictionary& parent, Dictionary&& subDict);


    // Public Member Functions

        // Access

            //- Access keyword
            std::string& keyword();
            const std::string& keyword() const;

            //- Access type
            typeEnum type() const;

            //- Access dictionary
            //  Not allocated if in stream entry mode
            Dictionary& dict();
            const Dictionary& dict() const;

            //- Access token stream
            //  Not valid if in dictionary mode
            const VecToken& tokens() const;


        // Query

            //- Type to sting
            static std::string typeEnumToString(typeEnum te);

            //- True if type is a dictionary entry
            bool isDict() const;

            //- True if type is a token entry
            bool isTokens() const;

            //- True if EOF was encountered, i.e. no further entries in istream
            bool isEof() const;

            //- True if neither dictionary nor tokens are allocated
            bool isNull() const;


        // Debug

            //- Debug write out to ostream
            void debugWrite(std::ostream& os) const;


    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const Entry& c);


private:

    // Private Member Functions

        //- Parse istream
        void read(std::istream& is, bool requireSemiColonEnd);

        //- Read keyword from istream
        //  Returns false if not valid, or not found
        bool getKeyword(std::istream& is);


    // Private Member Data

        //- Associated keyword
        std::string keyword_;

        //- Type of entry
        typeEnum type_;

        //- Dictionary data (dictionary mode)
        std::unique_ptr<Dictionary> dictPtr_;

        //- Token data (token mode)
        VecToken tokens_;

        //- Parent dictionary
        Dictionary& parent_;
    
};

} // end namespace

#endif // Entry_h