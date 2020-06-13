//
// Created by oreno on 6/13/2020.
//

#ifndef HW2_HASHTABLE_H
#define HW2_HASHTABLE_H
#include <iostream>
using std::cout;
using std::ostream;
using std::endl;

template<class T>
class HashTable{
private:
    int tableSize;
    int numOfElements;
    T* table;
    void ReallocTable();
    int hash1Func(T *item);
    int hash2Func(T *item);
public:
    explicit HashTable(int size);
    ~HashTable() = default;
    void Insert(T *item);
    void Delete(T* item);
    T* find(int itemId);
    void displayHash();
    friend ostream& operator<<(ostream& os, const T& item);
};

template<class T>
HashTable<T>::HashTable(int size) {
    tableSize = size;
    numOfElements = 0;
    table = new T*[tableSize]();
    for (int i = 0; i < tableSize ; ++i) {
        table[i] = NULL;
    }
}

template<class T>
void HashTable<T>::Insert(T *item) {
    int index = hashFunc(item);
    table[index] = item;
    numOfElements++;
    this->ReallocTable();

}

template<class T>
void HashTable<T>::Delete(T *item) {

}

template<class T>
T *HashTable<T>::find(int itemId) {
    return nullptr;
}

template<class T>
int HashTable<T>::hash1Func(T *item) {
    return *item % tableSize;
}

template<class T>
int HashTable<T>::hash2Func(T *item) {
    return *item;
}

template<class T>
void HashTable<T>::displayHash() {
    for (int i = 0; i < tableSize ; ++i) {
        cout << table[i] << endl;
    }

}

ostream &operator<<(ostream &os, const T &item) {
    cout << "printing table:" <<endl;
    displayHash();
    return os
}

template<class T>
void HashTable<T>::ReallocTable() {
    int newTableSize = 0;
    if (numOfElements == tableSize){
        newTableSize = tableSize*2;
    }
    if (numOfElements == tableSize/2){
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