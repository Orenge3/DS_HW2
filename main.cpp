#include <iostream>
#include "DS_AVL.h"
class test{
    int Id1;
    int Id2;


public:
    test(int Id1, int Id2):Id1(Id1),Id2(Id2){}
    test():Id1(0),Id2(0){}
    bool operator>(test& other){
        if (this->Id1>other.Id1){
            return true;
        } else {
            if (this->Id1<other.Id1) return false;
        }
        return this->Id2 > other.Id2;
    };
    bool operator<(test& other){
        if (this->Id1<other.Id1){
            return true;
        } else {
            if (this->Id1>other.Id1) return false;
        }
        return this->Id2 < other.Id2;
    };
    bool operator==(test& other){
        return this->Id1 == other.Id1 && this->Id2 == other.Id2;
    };
    bool operator!=(test& other){
        return !(this->Id1 == other.Id1 && this->Id2 == other.Id2);
    };


    test& operator=(const test& other){

        if(this!=&other){
            this->Id1=other.Id1;
            this->Id2=other.Id2;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const test& obj);


};

std::ostream &operator<<(std::ostream &os, const test &obj) {
    cout<<obj.Id1<<", "<<obj.Id2;
    return os;
}


int main() {
    int a=3;
    int b=4;
    int c=5;
    test t1(10,0);
    test t2(1,1);
    test t3(2,0);
    test t4(10,10);
    test t5(13,1);
    test t6(1,2);
    test t7(10,11);
    test t8(10,0);
    test t9(10,10);
    test t10(13,1);
    AVLTree<int,test> tree;


    tree.Insert(t1,a);
    tree.Insert(t2,b);
    tree.Insert(t3,c);
    tree.Insert(t4,c);
    tree.Insert(t5,c);
    tree.Insert(t6,c);
    tree.Insert(t7,c);
    tree.Insert(t8,c);
    tree.Insert(t9,c);
    tree.Insert(t10,c);
    tree.ObserveTree();
    cout<<tree.GetRankedObject(2)<<endl;
//    AVLTree<int,test> tree2;
//    tree2.ObserveTree();
//    tree2.GetRankedObject(0);
    return 0;
}