#pragma once
#include <string>


template<typename K>
class HashNode
{
public:
    K key;

    HashNode(K key)
    {
        this->key = key;
    }
};


class HashTable
{
    HashNode<std::string>** theArray;
    int capacity;
    int size;
    HashNode<std::string>* dummy;

public:

    HashTable();

    HashTable(int capacity);

    ~HashTable();

    int hashFunction(std::string key);

    void insertNode(std::string key);
    bool deleteNode(std::string key);

    int getPosition(std::string key);

    int sizeOfTable();

    bool isEmpty();

    void display();
    std::string displayToString();
};
