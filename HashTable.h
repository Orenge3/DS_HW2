//
// Created by oreno on 6/13/2020.
//

#ifndef HW2_HASHTABLE_H
#define HW2_HASHTABLE_H
#include <iostream>
using std::cout;
using std::ostream;
using std::endl;
#define PRIME 3
#define INIT_SIZE
typedef enum {
    HASH_SUCCESS = 0,
    HASH_FAILURE = -1,
    HASH_ALLOCATION_ERROR = -2,
    HASH_INVALID_INPUT = -3,
    HASH_NODE_NOT_FOUND=-4,
    HASH_EMPTY=-5
} HASH_RESULT;
template<class T>
class HashTable{ //double HASH Table
private:
    int tableSize;
    int numOfElements;
    T* table;
    void ReallocTable();
    int hash1Func(T item);
    int hash2Func(T item);
    bool isFull(){
        return (tableSize == numOfElements);
    }
    bool isQuarterFull(){
        return (tableSize/4 == numOfElements);
    }
public:
    explicit HashTable(int size);
    ~HashTable() = default;
    HASH_RESULT Insert(T item);
    HASH_RESULT Delete(T item);
    T find(int itemId);
    void displayHash();
    friend ostream& operator<<(ostream& os, const T& item);
};

template<class T>
HashTable<T>::HashTable(int size) {
    tableSize = size;
    numOfElements = 0;
    table = new T[tableSize]();
    for (int i = 0; i < tableSize ; ++i) {
        table[i] = NULL;
    }
}

template<class T>
HASH_RESULT HashTable<T>::Insert(T item) {
    int index = hash1Func(item);
    if (table[index] != NULL){ //collision occurs
        int index2 = hash2Func(item);
        int i = 1;
        while (true){//get NewIndex
            int newIndex = (index+i*index2) % tableSize;
            if (table[newIndex] == NULL){
                table[newIndex] = item;
                break;
            }
            i++;
        }
    } else //no collision
    table[index] = item;
    numOfElements++;
    this->ReallocTable();

}

template<class T>
HASH_RESULT HashTable<T>::Delete(T item) {

}

template<class T>
T HashTable<T>::find(int itemId) {
    return nullptr;
}

template<class T>
int HashTable<T>::hash1Func(T item) {
    return *item % tableSize;
}

template<class T>
int HashTable<T>::hash2Func(T item) {
    return 1+(*item % tableSize-PRIME);
}

template<class T>
void HashTable<T>::displayHash() {
    for (int i = 0; i < tableSize ; ++i) {
        cout << table[i] << endl;
    }

}

ostream &operator<<(ostream &os, const T &item) {
    cout << "printing table:" <<endl;
    return os
}

template<class T>
void HashTable<T>::ReallocTable() {
    int newTableSize = 0;
    if (isFull()){
        newTableSize = tableSize*2;
    }
    if (isQuarterFull()){
        newTableSize = tableSize/2;
    }
    if (newTableSize == 0){
        return;
    }
    T* newTable = new T[newTableSize]();
    for (int i = 0; i< tableSize ; ++i) {
        newTable[hashFunc(*table[i])]= table[i];
    }
    tableSize = newTableSize;
    delete [] table;
    table = newTable;
}


#endif //HW2_HASHTABLE_H
