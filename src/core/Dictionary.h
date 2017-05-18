#ifndef Dictionary_h
#define Dictionary_h

// Forward declarations

class Dictionary;
std::ostream& operator<<(std::ostream& os, const Dictionary& c);
std::istream& operator>>(std::istream& is, Dictionary& c);


class Dictionary:
	public std::list<Entry>
{
public:

	// Constructors

		//- Construct null
		Dictionary();

		//- Construct from istream
		Dictionary(std::istream& is);

	//- Destructor
	~Dictionary();


    // Friend functions
    friend std::ostream& operator<<(std::ostream& out, const Dictionary& c);
    friend std::istream& operator>>(std::istream& in, Dictionary& c);


private:

	// Private member data

		//- Hashed table for quick look-up
		HashTable<Entry*> hashedEntries_;

		//- Parent dictionary
		const Dictionary& parent_;
};

#endif
