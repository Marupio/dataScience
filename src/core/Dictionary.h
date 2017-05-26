#ifndef Dictionary_h
#define Dictionary_h

#include <list>
#include <unordered_map>
#include <Entry.h>

namespace ds {

// Forward declarations

class Dictionary;
std::ostream& operator<<(std::ostream& os, const Dictionary& c);
std::istream& operator>>(std::istream& is, Dictionary& c);


class Dictionary
{
public:

    // Public Data Types

        typedef std::list<Entry> ListEntry;
        typedef ListEntry::const_iterator const_iterator;
        typedef ListEntry::const_reverse_iterator const_reverse_iterator;
        typedef ListEntry::size_type size_type;
        typedef ListEntry::difference_type difference_type;


    // Static Member Data

        //- Null dictionary
        static const Dictionary null;

        //- Tab width
        static int tabWidth;


    // Constructors

        //- Construct null
        Dictionary();

        //- Construct base dictionary from istream
        Dictionary(std::istream& is);

        //- Construct from file
        Dictionary(std::string fileName);

        //- Construct embedded dictionary from istream and name
        Dictionary(std::string name, const Dictionary& parent, std::istream& is);

        // Rule of four
        // depth_ complicates construction, default ones will not do

            //- Copy constructor
            Dictionary(const Dictionary& dict);

            //- Move constructor
            Dictionary(Dictionary&& dict);

            //- Copy assignment operator
            Dictionary operator=(const Dictionary& dict);

            //- Move assignment operator
            Dictionary operator=(Dictionary&& dict);


    // Public Member Functions

        // Iterator access

            //- const_iterator begin
            const_iterator cbegin() const noexcept;

            //- const_iterator end
            const_iterator cend() const noexcept;

            //- const_reverse_iterator begin
            const_reverse_iterator crbegin() const noexcept;

            //- const_iterator end
            const_reverse_iterator crend() const noexcept;

            //- Return size
            size_type size() const noexcept;


        // Query

            //- Return parent
            const Dictionary& parent() const;

            //- Return full name: scopeName:keyName
            std::string name() const;

            //- Return scopeName
            const std::string& scopeName() const;

            //- Return keyName
            const std::string& keyName() const;

            //- Return depth
            int depth() const;

            //- True if keyword exists
            bool found(std::string keyword) const;

            //- Returns entry type, including unknown if not found
            Entry::typeEnum foundType(std::string keyword) const;

            //- Return associated entry, fail if not found
            const Entry& lookupEntry(std::string keyword) const;

            //- True if entry is a token stream, false if not found
            bool isTokens(std::string keyword) const;

            //- Lookup token stream
            const VecToken& lookupTokens(std::string keyword) const;

            //- True if entry is a sub-dictionary
            bool isDict(std::string keyword) const; 

            //- Lookup sub-dictionary
            Dictionary& lookupDict(std::string keyword);
            const Dictionary& lookupDict(std::string keyword) const;

            //- Lookup a templated type from the dictionary
            template<class T>
            T lookup(std::string keyword);

            //- Lookup a templated type from the dictionary, return defaultVal if not found
            template<class T>
            T lookupOrDefault(std::string keyword, T defaultVal);


        // Edit

            //- Add entry to dictionary
            void add(Entry&& e, bool overwrite=false);

            //- Add keyword and associated stream to dictionary
            void add(std::string keyword, std::istream& is, bool overwrite=false);

            //- Add keyword and string to be parsed to dictionary
            void add(std::string keyword, std::string parseThis, bool overwrite=false);

            //- Add string to dictionary (to be parsed by Entry)
            //  Use formats:
            //  * "keyword followed by other entries;"
            //  * "keyword { subdictionary entries or empty }"
            void add(std::string parseThis, bool overwrite=false);

            // Add subdictionary - since Entry must own Dictionary, move constructor only
            void add(std::string keyword, Dictionary&& subdict, bool overwrite=false);

            //- Add templated type to dictionary
            template<class T>
            void addType(std::string keyword, const T& val, bool overwrite=false);

            //- Erase an entry
            void erase(std::string keyword, bool failOnNotFound=false);

            //- Clear all entries
            void clear();


        // Debug

            //- Debug write
            void debugWrite(std::ostream& os) const;


    // Friend functions
    friend std::ostream& operator<<(std::ostream& os, const Dictionary& d);
    friend std::istream& operator>>(std::istream& is, Dictionary& d);


private:

    // Private Member Functions

        //- Read dictionary from istream
        void read(std::istream& is);


    // Private Member Data

        //- Parent dictionary
        const Dictionary& parent_;

        //- Associated file, with sub-dictionary context up to base level
        //  "/absolute/or/relative/path/to/file:subDictionary:subDictionary"
        std::string scopeName_;

        //- Keyword associated with this dictionary.  It's full name is scopeName:keyword
        std::string keyName_;

        //- Entry list, i.e. the data in the dictionary
        ListEntry entries_;
        
        //- Hashed table for quick look-up
        std::unordered_map<std::string, Entry*> hashedEntries_;

        //- Depth of sub-dictionary
        int depth_;
};

// Template Specialisations
template<>
bool ds::Dictionary::lookup(std::string keyword);

template<>
bool ds::Dictionary::lookupOrDefault(std::string keyword, bool defaultVal);


} // end namespace

#include <DictionaryTemplates.cpp>

#endif