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
#define INIT_SIZE 11
#define ITERATOR_UNINIT -1
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
    T deletedElement;
    int iteratorIndex = ITERATOR_UNINIT;
    T* table;
    void ReallocateTable(int newTableSize);
    int hash1Func(int itemId);
    int hash2Func(int itemId);
    bool isEmpty() {return numOfElements == 0;}
    bool isFull(){
        return (tableSize == numOfElements);
    }
    bool isQuarterFull(){
        return (tableSize/4 == numOfElements);
    }

    void ReHash();
public:
    HashTable() = delete;
    explicit HashTable(int size = INIT_SIZE);
    ~HashTable(){
        delete [] table;
        delete deletedElement;
    };
    HASH_RESULT Insert(T item);
    HASH_RESULT Delete(T item);
    HASH_RESULT TableDelete();
    T find(int itemId, int *rIndex =NULL);
    void displayHash();
    int GetNumOfElements(){return numOfElements;}
    int GetNumOfDeleted(){return deletedElements;}
    T HashGetFirst() {
        if (!isEmpty()){
            iteratorIndex = 0;
            while (iteratorIndex <= tableSize-1){
                if (table[iteratorIndex] != NULL && table[iteratorIndex] != deletedElement){
                    return table[iteratorIndex];
                } else{
                    iteratorIndex++;
                }
            }
            printf("problem with is empty function, or counting of elements");
        }
        iteratorIndex = ITERATOR_UNINIT;
        return NULL;
    }
    T HashGetNext(){
        while (iteratorIndex < tableSize-1 && iteratorIndex != ITERATOR_UNINIT){
            iteratorIndex++;
            if (table[iteratorIndex] != NULL && table[iteratorIndex] != deletedElement){
                return table[iteratorIndex];
            }
        }
        iteratorIndex = ITERATOR_UNINIT;
        return NULL;
    }
    int GetIndex(){return iteratorIndex;}
    int GetSize(){return tableSize;}

};

template<class T>
HashTable<T>::HashTable(int size) {
    tableSize = size;
    numOfElements = 0;
    deletedElements = 0;
    deletedElement = new Artist(0);
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
    iteratorIndex = ITERATOR_UNINIT;
    int index = hash1Func(**item);
    if (table[index] != NULL && table[index] != deletedElement){ //collision occurs
        if (*table[index] == *item){
            return HASH_FAILURE;
        }
        int index2 = hash2Func(**item);
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
        if(table[index] == deletedElement){
            deletedElements--;
        }
        table[index] = item;
    }
    numOfElements++;
    int newTableSize = 0;
    if (isFull()){
        newTableSize = tableSize * 2;
    }
    this->ReallocateTable(newTableSize);
    return HASH_SUCCESS;
}

template<class T>
HASH_RESULT HashTable<T>::Delete(T item) {
    iteratorIndex = ITERATOR_UNINIT;
    if (item == NULL){
        return HASH_INVALID_INPUT;
    }
    if (isEmpty()){
        return HASH_FAILURE;
    }
    int* index = new int ();
    T res = this->find(*(*item), index);
    if (res != NULL){
        table[*index] = deletedElement;
        delete index;
        numOfElements--;
        deletedElements++;
        if (deletedElements == tableSize/2){
            this->ReHash();
        }
        int newTableSize = 0;
        if (isQuarterFull()){
            newTableSize = tableSize / 2;
        }
        this->ReallocateTable(newTableSize);
        return HASH_SUCCESS;
    }
    delete index;
    return HASH_FAILURE;

}

template<class T>
HASH_RESULT HashTable<T>::TableDelete() {
    int numToDelete = numOfElements;
    for (int i = 0; i <= numToDelete; ++i) {
        for (int j = 0; j < tableSize ; ++j) {
            if (this->Delete(table[j]) == HASH_SUCCESS){
                i++;
            }
        }
    }
    return HASH_SUCCESS;
}

template<class T>
T HashTable<T>::find(int itemId, int *rIndex) {
    int index1 = hash1Func(itemId);
    int index2 = hash2Func(itemId);
    int i = 0;
    T tempItem = table[(index1 + i * index2) % tableSize];
    if (tempItem != NULL){
        while (**tempItem != itemId){
            i++;
            tempItem = table[(index1 + i * index2) % tableSize];
            if (tempItem == NULL){
                return NULL;
            }
        }
        if (rIndex != NULL){
            *rIndex = (index1 + i * index2) % tableSize;
        }
        return table[(index1 + i * index2) % tableSize];
    }
    return NULL;
}

template<class T>
int HashTable<T>::hash1Func(int itemId) {
    return itemId % tableSize;
}

template<class T>
int HashTable<T>::hash2Func(int itemId) {
    return 1+(itemId % (tableSize - PRIME));
}

template<class T>
void HashTable<T>::displayHash() {
    if (this->isEmpty()){
        cout << "Hash Table is Empty!" << endl;
        return;
    }
    cout << "Hash Table:" <<endl;
    for (int i = 0; i < tableSize ; ++i) {
        if (table[i] != NULL && *table[i] != *deletedElement){
            cout << i << "-->" << **table[i] << endl;
        } else
            cout << i << endl;
    }

}

template<class T>
void HashTable<T>::ReallocateTable(int newTableSize) {
    if (newTableSize == 0) return; //dont realloc if allocate to zero
    numOfElements = 0; //will reinsert them so zero counter
    deletedElements = 0;//after reallocating all deleted elements will be turned to null
    T* oldTable = table;
    int  oldTableSize = tableSize;
    T* newTable = new T[newTableSize]();
    table = newTable;
    tableSize = newTableSize;
    //initialize newTable
    for (int j = 0; j < newTableSize ; ++j) {
        table[j] = NULL;
    }
    for (int i = 0; i< oldTableSize ; ++i) {
        if (oldTable[i] != NULL && *oldTable[i] != *deletedElement){
            this->Insert(oldTable[i]);
        }
    }
    delete [] oldTable;
}

template<class T>
void HashTable<T>::ReHash() {
    T* temp = new T[tableSize]();
    numOfElements = 0; // will reinsert them
    for (int i = 0; i < tableSize; ++i) {
        temp[i] = table[i];
        table[i] = NULL;
    }
    for (int j = 0; j < tableSize; ++j) {
        if (temp[j] == NULL || *temp[j] == *deletedElement){
            continue;
        } else{
            this->Insert(temp[j]);
        }
    }
    delete [] temp;
    deletedElements = 0;
}

#define HASH_FOREACH(type, iterator, Hashtable)          \
    for(type iterator = (type) Hashtable.HashGetFirst() ; \
        iterator;                               \
        iterator = Hashtable.HashGetNext())


#endif //HW2_HASHTABLE_H
