#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <typeinfo>
#include <sstream>
#include <Windows.h>
#include <string>
#include "node.h"

#define NULL_POSITION INT_MAX


HashTable::HashTable()
{
    capacity = 20;
    size = 0;
    theArray = new HashNode<std::string> * [capacity];

    for (int i = 0; i < capacity; i++)
    {
        theArray[i] = NULL;
    }

    dummy = new HashNode<std::string>("");
}

HashTable::HashTable(int capacity)
{
    this->capacity = capacity;
    size = 0;
    theArray = new HashNode<std::string> * [capacity];

    for (int i = 0; i < capacity; i++)
    {
        theArray[i] = NULL;
    }

    dummy = new HashNode<std::string>("");
}

HashTable::~HashTable()
{
    for (int i = 0; i < capacity; i++)
    {
        if (NULL != theArray[i])
        {
            delete theArray[i];
        }
    }
    delete[] theArray;
}

int HashTable::hashFunction(std::string key)
{
    int subhash = 0;
    for (unsigned int i = 0; i < key.size(); i++)
    {
        subhash = subhash + int(key[i]);
        subhash = subhash % capacity;
    }
    return subhash;
}

void HashTable::insertNode(std::string key)
{
    HashNode<std::string>* temp = new HashNode<std::string>(key);

    int hashIndex = hashFunction(key);

    while (theArray[hashIndex] != NULL && 0 != theArray[hashIndex]->key.compare(key) && theArray[hashIndex]->key.size() != 1)
    {
        hashIndex++;
        hashIndex %= capacity;
    }

    if (theArray[hashIndex] == NULL || theArray[hashIndex]->key.size() == 0)
    {
        size++;
    }
    theArray[hashIndex] = temp;
}

bool HashTable::deleteNode(std::string key)
{
    int hashIndex = hashFunction(key);

    while (theArray[hashIndex] != NULL)
    {
        if (theArray[hashIndex]->key == key)
        {
            HashNode<std::string>* temp = theArray[hashIndex];
            theArray[hashIndex] = dummy;
            size--;
            return TRUE;
        }
        hashIndex++;
        hashIndex %= capacity;
    }
    return NULL;
}

int HashTable::getPosition(std::string key)
{
    int hashIndex = hashFunction(key);
    int counter = 0;

    while (theArray[hashIndex] != NULL)
    {
        int counter = 0;
        if (counter++ > capacity)
        {
            return NULL_POSITION;
        }

        if (0 == theArray[hashIndex]->key.compare(key))
        {
            return hashIndex;
        }
        hashIndex++;
        hashIndex %= capacity;
    }
    return NULL_POSITION;
}

int HashTable::sizeOfTable()
{
    return size;
}

bool HashTable::isEmpty()
{
    return size == 0;
}

void HashTable::display()
{

    for (int i = 0; i < capacity; i++)
    {
        if (theArray[i] != NULL && theArray[i]->key.size() != 0)
        {
            std::cout << "key-position = { " << theArray[i]->key
                << ", " << i
                << " }"
                << std::endl;
        }
    }
}

std::string HashTable::displayToString()
{
    std::string theString;
    std::stringstream aux;
    for (int i = 0; i < capacity; i++)
    {
        if (theArray[i] != NULL && theArray[i]->key.size() != 0)
        {
            aux << "key-position = { " << theArray[i]->key
                << ", " << i
                << " }"
                << std::endl;
        }
    }
    theString = aux.str();
    return theString;
}

