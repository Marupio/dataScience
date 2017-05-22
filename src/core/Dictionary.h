#ifndef Dictionary_h
#define Dictionary_h

#include <unordered_map>
#include <VecToken.h>

// Forward declarations

class Dictionary;
std::ostream& operator<<(std::ostream& os, const Dictionary& c);
std::istream& operator>>(std::istream& is, Dictionary& c);


class Dictionary:
    public std::list<Entry>
{
public:

    // Static Member Data

        //- Null dictionary
        static Dictionary null;


    // Constructors

        //- Construct null
        Dictionary();

        //- Construct base dictionary from istream
        Dictionary(std::istream& is);

        //- Construct from file
        Dictionary(const std::string& fileName);

        //- Construct embedded dictionary from istream and name
        Dictionary(const std::string& name, const Dictionary& parent, std::istream& is);


    // Public Member Functions

        //- Return file name
        const std::string& name() const;

        //- Add entry to dictionary
        void add(Entry&&);

        //- True if keyword exists
        bool found(const std::string& keyword) const;

        //- Returns entry type, including unknown if not found
        Entry::typeEnum foundType(const std::string& keyword) const;

        //- Return associated entry
        const Entry& lookup(const std::string& keyword) const;

        //- True if entry is a token stream
        bool isTokens(const std::string& keyword) const;

        //- Lookup token stream
        VecToken& lookupTokens(const std::string& keyword) const;

        //- True if entry is a sub-dictionary
        bool isDict(const std::string& keyword) const; 

        //- Lookup sub-dictionary
        Dictionary& lookupDict(const std::string& keyword) const;


    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const Dictionary& c);
    friend std::istream& operator>>(std::istream& in, Dictionary& c);


private:

    // Private Member Functions

        //- Read dictionary from istream
        read(std::istream& is);


    // Private Member Data

        //- Parent dictionary
        const Dictionary& parent_;

        //- Associated file, with sub-dictionary context:
        //  "/absolute/or/relative/path/to/file:subDictionary:subDictionary"
        std::string name_;

        //- Hashed table for quick look-up
        std::unordered_map<std::string, Entry*> hashedEntries_;
};

#endif