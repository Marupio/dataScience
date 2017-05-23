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

            //- Return file name
            const std::string& name() const;

            //- True if keyword exists
            bool found(const std::string& keyword) const;

            //- Returns entry type, including unknown if not found
            Entry::typeEnum foundType(const std::string& keyword) const;

            //- Return associated entry, fail if not found
            const Entry& lookup(const std::string& keyword) const;

            //- True if entry is a token stream, false if not found
            bool isTokens(const std::string& keyword) const;

            //- Lookup token stream
            const VecToken& lookupTokens(const std::string& keyword) const;

            //- True if entry is a sub-dictionary
            bool isDict(const std::string& keyword) const; 

            //- Lookup sub-dictionary
            Dictionary& lookupDict(const std::string& keyword);
            const Dictionary& lookupDict(const std::string& keyword) const;


        // Edit

            //- Add entry to dictionary
            void add(Entry&& e, bool overwrite=false);

            //- Add type to dictionary
            template<class T>
            void add(std::string& keyword, const T& val, bool overwrite=false);

            //- Erase an entry
            void erase(const std::string& keyword, bool failOnNotFound=false);


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

        //- Associated file, with sub-dictionary context:
        //  "/absolute/or/relative/path/to/file:subDictionary:subDictionary"
        std::string name_;

        //- Entry list, i.e. the data in the dictionary
        ListEntry entries_;
        
        //- Hashed table for quick look-up
        std::unordered_map<std::string, Entry*> hashedEntries_;
};

} // end namespace

#include <DictionaryTemplates.cpp>

#endif