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
#define DELETED -1
#define INIT_SIZE
typedef enum {
    HASH_SUCCESS = 0,
    HASH_FAILURE = -1,
    HASH_ALLOCATION_ERROR = -2,
    HASH_INVALID_INPUT = -3,
    HASH_ITEM_NOT_FOUND=-4,
    HASH_EMPTY=-5
} HASH_RESULT;
template<class T>
class HashTable{ //double HASH Table
private:
    int tableSize;
    int numOfElements;
    int deletedElements;
    T* table;
    void ReallocateTable();
    int hash1Func(int itemId);
    int hash2Func(int itemId);
    bool isFull(){
        return (tableSize == numOfElements);
    }
    bool isQuarterFull(){
        return (tableSize/4 == numOfElements);
    }
    void ReHash();
public:
    explicit HashTable(int size);
    ~HashTable() = default;
    HASH_RESULT Insert(T item);
    HASH_RESULT Delete(T item);
    T find(int itemId);
    void displayHash();
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
    if (item == NULL){
        return HASH_INVALID_INPUT;
    }
    int index = hash1Func(*item);
    if (table[index] != NULL){ //collision occurs
        if (table[index] == item){
            return HASH_FAILURE;
        }
        int index2 = hash2Func(*item);
        int i = 1;
        while (true){//get NewIndex
            int newIndex = (index + i * index2) % tableSize;
            if (table[newIndex] == NULL){
                table[newIndex] = item;
                break;
            }
            if (table[newIndex] == item){
                return HASH_FAILURE;
            }
            i++;
        }
    }else{//no collision
        table[index] = item;
        numOfElements++;
        this->ReallocateTable();
        return HASH_SUCCESS;
    }
}

template<class T>
HASH_RESULT HashTable<T>::Delete(T item) {
    if (item == NULL){
        return HASH_INVALID_INPUT;
    }
    T res = this->find(*item);
    if (res != NULL){
        table[*item] = DELETED;
        delete item;
        numOfElements--;
        deletedElements++;
        ReallocateTable();
        return HASH_SUCCESS;
    }
    return HASH_FAILURE;

}

template<class T>
T HashTable<T>::find(int itemId) {
    int index1 = hash1Func(itemId);
    int index2 = hash2Func(itemId);
    int i = 0;
    while (table[(index1 + i * index2) % tableSize] != itemId){
        if (table[(index1 + i * index2) % tableSize] == NULL){
            return NULL;
        }
        i++;
    }
    return table[(index1 + i * index2) % tableSize];
}

template<class T>
int HashTable<T>::hash1Func(int itemId) {
    return itemId % tableSize;
}

template<class T>
int HashTable<T>::hash2Func(int itemId) {
    return 1+(itemId % tableSize - PRIME);
}

template<class T>
void HashTable<T>::displayHash() {
    for (int i = 0; i < tableSize ; ++i) {
        if (table[i] != NULL){
            cout << i << "-->" <<table[i] << endl;
        } else
            cout << i << endl;
    }

}

template<class T>
void HashTable<T>::ReallocateTable() {
    int oldTableSize = tableSize;
    T* oldTable = table;
    if (isFull()){
        tableSize *= 2;
    }
    if (isQuarterFull()){
        tableSize /= 2;
    }
    if (oldTableSize == tableSize){
        if (deletedElements == numOfElements){
            this->ReHash();
        }
        return;
    }
    T* newTable = new T[tableSize]();
    newTable->tableSize = tableSize;
    table = newTable;
    for (int i = 0; i< oldTableSize ; ++i) {
        if (oldTable[i] != NULL && oldTable[i] != DELETED){
            this->Insert(oldTable[i]);
        }
        table[i] = NULL;
    }
    delete [] oldTable;
}

template<class T>
void HashTable<T>::ReHash() {
    T* temp = new T[tableSize]();
    for (int i = 0; i < tableSize; ++i) {
        temp[i] =table[i];
        table[i] = NULL;
    }
    for (int j = 0; j < tableSize; ++j) {
        if (temp[j] == DELETED || temp[j] == NULL){
            continue;
        } else{
            this->Insert(temp[j]);
        }
    }

}


#endif //HW2_HASHTABLE_H
