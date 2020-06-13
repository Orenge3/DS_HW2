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
class hashTable{
private:
    int tableSize;
    int numOfElements;
    T* table;
public:
    explicit hashTable(int size);
    ~hashTable() = default;
    void  Insert(T* item, int itemId);
    void Delete(T* item);
    T* find(int itemId);
    int hashFunc(T *item);
    void displayHash();
    void ReallocTable();
    friend ostream& operator<<(ostream& os, const T& item);
};

template<class T>
hashTable<T>::hashTable(int size) {
    tableSize = size;
    numOfElements = 0;
    table = new T*[tableSize]();
}

template<class T>
void hashTable<T>::Insert(T *item, int itemId) {
    numOfElements++;
    this->ReallocTable();
    int key = hashFunc(itemId);
    table[key] = item;

}

template<class T>
void hashTable<T>::Delete(T *item) {

}

template<class T>
T *hashTable<T>::find(int itemId) {
    return nullptr;
}

template<class T>
int hashTable<T>::hashFunc(T *item) {
    return 0;
}

template<class T>
void hashTable<T>::displayHash() {
    for (int i = 0; i < tableSize ; ++i) {
        cout << table[i] << endl;
    }

}

ostream &operator<<(ostream &os, const T &item) {
    return <#initializer#>;
}

template<class T>
void hashTable<T>::ReallocTable() {
    if (numOfElements == tableSize){
        T* newTable;
        newTable = new T[2*tableSize]();
        for (int i = 0; i< tableSize ; ++i) {
            hashFunc(*table[i]);
            newTable[]
        }

    }
    if (numOfElements == tableSize/2){

    }
}


#endif //HW2_HASHTABLE_H
