//  Originally Created by Kar Beringer on June 18, 2014.
//  Modified by David L. F. Gaden
//  Copyright (C) 2017 David L. F. Gaden

#include "HashTable.h"

// ****** Constructors ****** //

ds::HashTable::HashTable(int tableLength) {
    if (tableLength <= 0) tableLength = 13;
    array = new LinkedList[ tableLength ];
    length = tableLength;
}


// ****** Destructor ****** //

ds::HashTable::~HashTable() {
    delete [] array;
}


// ****** Public Member Functions ****** //

int ds::HashTable::hash(string itemKey) {
    int value = 0;
    for (int i = 0; i < itemKey.length(); i++)
        value += itemKey[i];
    return (value * itemKey.length()) % length;
}


void ds::HashTable::insertItem(Item * newItem) {
    int index = hash(newItem -> key);
    array[ index ].insertItem(newItem);
}


bool ds::HashTable::removeItem(string itemKey) {
    int index = hash(itemKey);
    return array[ index ].removeItem(itemKey);
}


ds::Item * ds::HashTable::getItemByKey(string itemKey) {
    int index = hash(itemKey);
    return array[ index ].getItem(itemKey);
}


void ds::HashTable::printTable() {
    cout << "\n\nHash Table:\n";
    for (int i = 0; i < length; i++)
    {
        cout << "Bucket " << i + 1 << ": ";
        array[i].printList();
    }
}


void ds::HashTable::printHistogram() {
    cout << "\n\nHash Table Contains ";
    cout << getNumberOfItems() << " Items total\n";
    for (int i = 0; i < length; i++)
    {
        cout << i + 1 << ":\t";
        for (int j = 0; j < array[i].getLength(); j++)
            cout << " X";
        cout << "\n";
    }
}


int ds::HashTable::getLength() {
    return length;
}


int ds::HashTable::getNumberOfItems() {
    int itemCount = 0;
    for (int i = 0; i < length; i++)
    {
        itemCount += array[i].getLength();
    }
    return itemCount;
}

// ****** END ****** //