//  Originally Created by Kar Beringer on June 18, 2014.
//  Modified by David L. F. Gaden
//  Copyright (C) 2017 David L. F. Gaden
#ifndef HashTable_h
#define HashTable_h

#include "LinkedList.h"

namespace ds {

//*****************************************************************
// Hash Table objects store a fixed number of Linked Lists.
//*****************************************************************
class HashTable {
public:

    //- Construct null
    HashTable(int tableLength = 13);

    //- Destructor
    ~HashTable();
    

    // Public Member Functions

        //- Add an item to the Hash Table.
        void insertItem(Item * newItem);
        
        // Delete an Item by key from the Hash Table.
        // Returns true if the operation is successful.
        bool removeItem(std::string itemKey);
        
        // Return an item from the Hash Table by key.
        // If the item isn't found, a null pointer is returned.
        Item * getItemByKey(std::string itemKey);
        
        // Display the contents of the Hash Table to console window.
        void printTable();
        
        // Print a histogram illustrating the Item distribution.
        void printHistogram();
        
        // Return the number of locations in the Hash Table.
        int getLength();
        
        // Return the number of Items in the Hash Table.
        int getNumberOfItems();
    


private:
    
    // Array is a reference to an array of Linked Lists.
    LinkedList * array;
    
    // Length is the size of the Hash Table array.
    int length;
    
    // Returns an array location for a given item key.
    int hash(std::string itemKey);
    

};

} // end namespace

#endif