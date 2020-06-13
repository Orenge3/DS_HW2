/****************************************************************************/
/*                                                                          */
/* Template for AVL tree data struct                                        */
/*                                                                          */
/****************************************************************************/
#include <stdio.h>
#include <new>
#include "iostream"
#include <stdexcept>
#ifndef INC_1W_DS_AVL_H
#define INC_1W_DS_AVL_H

using namespace std;
typedef enum {
    AVL_SUCCESS = 0,
    AVL_FAILURE = -1,
    AVL_ALLOCATION_ERROR = -2,
    AVL_INVALID_INPUT = -3,
    AVL_NODE_NOT_FOUND=-4,
    AVL_EMPTY=-5
} AVL_RESULT;
typedef int sideHeightInt;
template <class T, class ID>
class AVLTree{
private:
    class node{

    public:
        ID nodeID;
        T* _data;
        node* leftChild;
        node* rightChild;
        node* parent;
        int height;
        int subTreeNodeNumber;
        node(const ID nodeID, T data) : nodeID(nodeID), leftChild(nullptr), rightChild(nullptr), parent(nullptr), height(0),subTreeNodeNumber(0) {
            _data=new T;
            *_data=data;
        }

        node() : leftChild(nullptr), rightChild(nullptr), parent(nullptr), height(0),subTreeNodeNumber(0) {}

        AVL_RESULT findNode(AVLTree::node** requestedNode, ID nodeID, AVLTree *tree, AVLTree::node** parentNode=NULL);
        AVL_RESULT findNodeRecursion(AVLTree::node** requestedNode,ID nodeID, node** parentNode);
        void postOrderTreeDeleteRecursion(node* nodeToDelete);
        void postOrderTreeObserve(AVLTree::node *node);
        void correctTree(ID requestedNodeID, node* parentNode);
        sideHeightInt sideHeight(node* childNode);
        void swapNodes(node* fNode, node* sNode);
        void correctionLL(node *parentNode);
        void correctionRR(node *parentNode);
        void correctionLR(node *parentNode);
        void correctionRL(node *parentNode);
        void deleteNode(node* nodeToDelete, node* parentToNodeToDelete, AVLTree* tree);
        node* deleteNodeRecursion(node* parentNode, node* nodeToDelete);
        int getNodeHeight(node* a, node* b);
        int getSubtreeNodeNumber(node* a, node* b);
        bool BFisOkay(node* parentNode);
        void findSmallestNode(AVLTree* tree);
        int printTreeInorderRecursion(int numberOfNodesToPrint, node* nodeToPrint);
        ID findRankedNodeRecursion(int rank, int parentRank, node* currentNode);
    };
    int treeSize=0;
    node* smallestNode= nullptr;
    node* treeEntry= nullptr;
public:

    AVL_RESULT Find(ID objectID, T& data);
    AVL_RESULT Insert(ID objectID, T& data);
    AVL_RESULT Delete(ID objectID);
    bool IsEmpty();
    void DeleteTree();
    void ObserveTree();
    ID GetRankedObject(int rank);
    int PrintTreeInorderFast(int numberOfNodesToPrint);

};


template <class T, class ID>
AVL_RESULT AVLTree<T,ID>::Find(ID objectID, T& data) {
    node** reqNode=new node*;

    if((*reqNode)->findNode(reqNode, objectID,this)==AVL_SUCCESS) {
        data = *(*reqNode)->_data;

        delete(reqNode);
        return AVL_SUCCESS;
    }
    delete(reqNode);
    return AVL_NODE_NOT_FOUND;
}

template <class T, class ID>
ID AVLTree<T,ID>::GetRankedObject(int rank) {
    if (rank<1 || rank>treeSize) throw std::runtime_error("AVL_Error: rank is not natural number or it is bigger than size of tree!");
    return this->treeEntry->findRankedNodeRecursion(rank, 0, this->treeEntry);

}

template <class T, class ID>
ID AVLTree<T,ID>::node::findRankedNodeRecursion(int rank, int parentRank, node* currentNode){
    int currentRank;
    int rightNodeAmount;
    int leftNodeAmount;
    
    if(currentNode->leftChild== nullptr){
        leftNodeAmount=0;
    } else leftNodeAmount=currentNode->leftChild->subTreeNodeNumber;
    if(currentNode->rightChild== nullptr){
        rightNodeAmount=0;
    } else rightNodeAmount=currentNode->rightChild->subTreeNodeNumber;
if(parentRank!=0){
    if(currentNode->parent->nodeID<currentNode->nodeID){
        currentRank=parentRank-leftNodeAmount-1;
    }else{
        currentRank=parentRank+rightNodeAmount+1;
    }
} else currentRank=rightNodeAmount+1;

    
    if (currentRank==rank) return currentNode->nodeID;
    if(currentRank<rank) return findRankedNodeRecursion(rank, currentRank, currentNode->leftChild);
    return findRankedNodeRecursion(rank, currentRank, currentNode->rightChild);
}


template <class T, class ID>
AVL_RESULT AVLTree<T,ID>::Insert(ID objectID, T &data) {

    try{
        node* newNode = new node(objectID, data);
        node** nodePointer = new node*;
        node** parentNodePointer = new node*;

        switch(newNode->findNode(nodePointer, objectID, this ,parentNodePointer)){

            case AVL_EMPTY:
                treeEntry=newNode;
                this->smallestNode=newNode;

                delete(parentNodePointer);
                delete(nodePointer);
                treeSize++;
                return AVL_SUCCESS;

            case AVL_NODE_NOT_FOUND:

                if((*nodePointer)->nodeID>objectID) {
                    (*nodePointer)->leftChild=newNode;
                    (*nodePointer)->leftChild->parent=(*nodePointer);

                } else {
                    (*nodePointer)->rightChild=newNode;
                    (*nodePointer)->rightChild->parent=(*nodePointer);
                }


                delete(parentNodePointer);
                delete(nodePointer);
                newNode->correctTree(objectID, treeEntry);
                newNode->findSmallestNode(this);
                treeSize++;
                return AVL_SUCCESS;
            default:
                delete(newNode->_data);
                delete(newNode);
                delete(parentNodePointer);
                delete(nodePointer);
                return AVL_FAILURE;
        }

    }
    catch (const std::bad_alloc& e){

        return AVL_ALLOCATION_ERROR;
    }
}

template <class T, class ID>
AVL_RESULT AVLTree<T,ID>::Delete(ID objectID) {
    node** nodePointer = new node*;
    node** parentNode = new node*;
    if((*nodePointer)->findNode(nodePointer, objectID, this, parentNode)!=AVL_SUCCESS) {
        delete (parentNode);
        delete(nodePointer);
        return AVL_NODE_NOT_FOUND;
    }
    ID deleteNodeID=(*nodePointer)->nodeID;

    (*nodePointer)->deleteNode(*nodePointer, *parentNode, this);
    (*nodePointer)->findSmallestNode(this);
    delete(nodePointer);
    delete(parentNode);
    treeSize--;
    return AVL_SUCCESS;
}

template <class T, class ID>
void AVLTree<T,ID>::node::deleteNode(node* nodeToDelete, node* parentToNodeToDelete, AVLTree *tree) {

    node* nodeToCheck;
    if(nodeToDelete->leftChild==NULL){


        if(nodeToDelete->rightChild==NULL){

            if(tree->treeEntry==nodeToDelete){

                delete(nodeToDelete->_data);
                delete(nodeToDelete);
                tree->treeEntry= nullptr;
                tree->smallestNode= nullptr;
                return;
            }


            if(parentToNodeToDelete->nodeID<nodeToDelete->nodeID){
                parentToNodeToDelete->rightChild= nullptr;
                if(parentToNodeToDelete->leftChild==NULL){
                    nodeToCheck=parentToNodeToDelete;
                } else nodeToCheck=parentToNodeToDelete->leftChild;

                delete(nodeToDelete->_data);
                delete(nodeToDelete);
            } else{
                parentToNodeToDelete->leftChild= nullptr;
                if(parentToNodeToDelete->rightChild==NULL){
                    nodeToCheck=parentToNodeToDelete;
                } else nodeToCheck=parentToNodeToDelete->rightChild;
                delete(nodeToDelete->_data);
                delete(nodeToDelete);
            }

        }else{
            if(parentToNodeToDelete==NULL){
                tree->treeEntry=nodeToDelete->rightChild;
                if (nodeToDelete->rightChild!=NULL) nodeToDelete->rightChild->parent= nullptr;
                delete(nodeToDelete->_data);
                delete(nodeToDelete);
                return;
            }

            swapNodes(nodeToDelete, nodeToDelete->rightChild);
            node* temp = nodeToDelete->rightChild;

            nodeToDelete->rightChild=temp->rightChild;
            if (nodeToDelete->rightChild!=NULL) nodeToDelete->rightChild->parent=nodeToDelete;



            if(nodeToDelete->rightChild!=NULL){
                nodeToCheck=nodeToDelete->rightChild;
            }else if(nodeToDelete->leftChild!=NULL){
                nodeToCheck=nodeToDelete->leftChild;
            } else nodeToCheck=nodeToDelete;

            delete(temp->_data);
            delete(temp);
        }
    } else{
        if(nodeToDelete->leftChild->rightChild==NULL){

            if(parentToNodeToDelete==NULL){
                tree->treeEntry=nodeToDelete->leftChild;
                if (nodeToDelete->leftChild!=NULL) nodeToDelete->leftChild->parent= nullptr;
                nodeToDelete->leftChild->rightChild=nodeToDelete->rightChild;
                if (nodeToDelete->leftChild->rightChild!=NULL) nodeToDelete->leftChild->rightChild->parent=nodeToDelete->leftChild;

                delete(nodeToDelete->_data);
                delete(nodeToDelete);
                return;
            }

            swapNodes(nodeToDelete,nodeToDelete->leftChild);

            node* temporary = nodeToDelete->leftChild;

            nodeToDelete->subTreeNodeNumber=temporary->subTreeNodeNumber;
            nodeToDelete->height=temporary->height;
            nodeToDelete->leftChild=temporary->leftChild;
            if(nodeToDelete->leftChild!=NULL) nodeToDelete->leftChild->parent=nodeToDelete;

            if(nodeToDelete->leftChild!=NULL){
                nodeToCheck=nodeToDelete->leftChild;
            }else if(nodeToDelete->rightChild!=NULL){
                nodeToCheck=nodeToDelete->rightChild;
            }else nodeToCheck=nodeToDelete;


            delete(temporary->_data);
            delete(temporary);
        }else
        {

            nodeToCheck=deleteNodeRecursion(nodeToDelete->leftChild,nodeToDelete);
            if(nodeToCheck->leftChild!=NULL) nodeToCheck=nodeToCheck->leftChild;
        }
    }


    correctTree(nodeToCheck->nodeID,tree->treeEntry);
}


template <class T, class ID>
typename AVLTree<T,ID>::node* AVLTree<T,ID>::node::deleteNodeRecursion(node* parentNode, node* nodeToDelete) {

    if(parentNode->rightChild->rightChild==NULL){
        swapNodes(nodeToDelete,parentNode->rightChild);
        node* temp = parentNode->rightChild;
        if (parentNode->rightChild->leftChild!=NULL){
            parentNode->rightChild=parentNode->rightChild->leftChild;
            if(parentNode->rightChild!= NULL) parentNode->rightChild->parent=parentNode;

        } else parentNode->rightChild= nullptr;
        delete (temp->_data);
        delete(temp);
        return parentNode;

    }
    return deleteNodeRecursion( parentNode->rightChild, nodeToDelete);
}



template <class T, class ID>
void AVLTree<T,ID>::DeleteTree() {
    node* Node;
    Node->postOrderTreeDeleteRecursion(this->treeEntry);
    this->AVLTree<T,ID>::treeEntry= nullptr;
    this->AVLTree<T,ID>::smallestNode= nullptr;
    cout<<"DeleteTree: Tree is deleted"<<endl;
}

template <class T, class ID>
void AVLTree<T,ID>::ObserveTree() {
    cout<<"***************************"<<endl;
    cout<<"Tree summary:"<<endl;
    cout<<"Size of tree: "<<treeSize<<endl;
    cout<<"***************************"<<endl;
    this->treeEntry->postOrderTreeObserve(this->treeEntry);
    cout<<"***************************"<<endl;
}

template <class T, class ID>
int AVLTree<T,ID>::PrintTreeInorderFast(int numberOfNodesToPrint) {
    node* nodeToPrint = this->smallestNode;
    cout<<"Printing tree inorder:"<<endl;
    cout<<"-------------------------"<<endl;
    while (nodeToPrint!= nullptr && numberOfNodesToPrint>0){
        cout<<nodeToPrint->nodeID<<" ";
        numberOfNodesToPrint--;
        numberOfNodesToPrint=nodeToPrint->printTreeInorderRecursion(numberOfNodesToPrint, nodeToPrint->rightChild);
        nodeToPrint=nodeToPrint->parent;
    }
    cout<<"\n-------------------------"<<endl;

    return numberOfNodesToPrint;
}


template <class T, class ID>
bool AVLTree<T,ID>::IsEmpty() {
    return this->treeEntry== nullptr;
}

template <class T, class ID>
int AVLTree<T,ID>::node::printTreeInorderRecursion(int numberOfNodesToPrint, AVLTree::node *nodeToPrint) {
    if (nodeToPrint== nullptr || numberOfNodesToPrint==0) return  numberOfNodesToPrint;

    numberOfNodesToPrint=printTreeInorderRecursion(numberOfNodesToPrint, nodeToPrint->leftChild);

    cout<<nodeToPrint->nodeID<<" ";
    numberOfNodesToPrint=printTreeInorderRecursion(numberOfNodesToPrint-1, nodeToPrint->rightChild);
    return numberOfNodesToPrint;
}

template <class T, class ID>
AVL_RESULT AVLTree<T,ID>::node::findNode(AVLTree::node** requestedNode, ID requestedNodeID, AVLTree* tree, AVLTree::node** parentNode){

    if(tree->treeEntry== nullptr){
        return AVL_EMPTY;
    }

    if(tree->treeEntry->nodeID==requestedNodeID){
        *requestedNode=tree->treeEntry;
        return AVL_SUCCESS;
    }
    bool deleteParent=false;
    if(parentNode==NULL) {
        parentNode=new node*;
        deleteParent=true;
    }
    (*parentNode)=tree->treeEntry;
    AVL_RESULT result =  findNodeRecursion(requestedNode,requestedNodeID, parentNode);
    if(deleteParent){
        delete(parentNode);
        return result;
    }
    return result;

}

template <class T, class ID>
AVL_RESULT AVLTree<T,ID>::node::findNodeRecursion(AVLTree::node** requestedNode, ID requestedNodeID, AVLTree::node** parentNode) {

    if ((*parentNode)->nodeID<requestedNodeID){
        if ((*parentNode)->rightChild==NULL){
            *requestedNode=(*parentNode);
            return AVL_NODE_NOT_FOUND;
        }
        if ((*parentNode)->rightChild->nodeID==requestedNodeID){

            *requestedNode=(*parentNode)->rightChild;
            return AVL_SUCCESS;
        }
        (*parentNode)=(*parentNode)->rightChild;
        return findNodeRecursion(requestedNode, requestedNodeID, parentNode);

    }
    if ((*parentNode)->nodeID>requestedNodeID){
        if ((*parentNode)->leftChild==NULL){
            *requestedNode=(*parentNode);
            return AVL_NODE_NOT_FOUND;
        }
        if ((*parentNode)->leftChild->nodeID==requestedNodeID){
            *requestedNode=(*parentNode)->leftChild;
            return AVL_SUCCESS;
        }
        (*parentNode)=(*parentNode)->leftChild;
        return findNodeRecursion(requestedNode, requestedNodeID, parentNode);

    }

    return AVL_FAILURE;//Don't suppose to reach this

}

template <class T, class ID>
void AVLTree<T,ID>::node::postOrderTreeDeleteRecursion(AVLTree::node *nodeToDelete) {
    if (nodeToDelete==NULL) return;
    postOrderTreeDeleteRecursion(nodeToDelete->leftChild);
    postOrderTreeDeleteRecursion(nodeToDelete->rightChild);
    delete(nodeToDelete->_data);
    delete(nodeToDelete);
}
template <class T, class ID>
void AVLTree<T,ID>::node::postOrderTreeObserve(AVLTree::node *node) {
    if (node==NULL) return;
    postOrderTreeObserve(node->leftChild);
    postOrderTreeObserve(node->rightChild);
    cout<<"Node ID: "<<node->nodeID<<endl;
    cout<<"Node height: "<<node->height<<endl;
    cout<<"Node subtree number: "<<node->subTreeNodeNumber<<endl;
    if(node->leftChild!=NULL) {
        cout<<"Node left child: "<<node->leftChild->nodeID<<endl;
    }else cout<<"Node left child: NULL"<<endl;
    if(node->rightChild!=NULL) {
        cout<<"Node right child: "<<node->rightChild->nodeID<<endl;
    }else cout<<"Node right child: NULL"<<endl;
    cout<<"----------------------------"<<endl;
    if(!BFisOkay(node)) throw;


}
template <class T, class ID>
int AVLTree<T,ID>::node::getNodeHeight(node* a, node* b){
    int a_height;
    int b_height;
    if (a==NULL) {
        a_height = 0;
    }else a_height=a->height+1;

    if (b==NULL) {
        b_height = 0;
    }
    else b_height=b->height+1;

    return a_height>b_height ? a_height : b_height;
}

template <class T, class ID>
int AVLTree<T,ID>::node::getSubtreeNodeNumber(node* a, node* b){
    int a_node_number;
    int b_node_number;
    if (a==NULL) {
        a_node_number = 0;
    }else a_node_number=a->subTreeNodeNumber;

    if (b==NULL) {
        b_node_number = 0;
    }
    else b_node_number=b->subTreeNodeNumber;

    return a_node_number+b_node_number+1;
}
template <class T, class ID>
void AVLTree<T,ID>::node::correctTree(ID requestedNodeID, node* parentNode){
    if(parentNode->nodeID>requestedNodeID){
        correctTree(requestedNodeID, parentNode->leftChild);
        parentNode->height=getNodeHeight(parentNode->rightChild,parentNode->leftChild);
        parentNode->subTreeNodeNumber=getSubtreeNodeNumber(parentNode->rightChild,parentNode->leftChild);

        if(parentNode->height >= parentNode->leftChild->height+1 && BFisOkay(parentNode)){
            return;
        }

    }

    if(parentNode->nodeID<requestedNodeID){
        correctTree(requestedNodeID, parentNode->rightChild) ;
        parentNode->height=getNodeHeight(parentNode->rightChild,parentNode->leftChild);
        parentNode->subTreeNodeNumber=getSubtreeNodeNumber(parentNode->rightChild,parentNode->leftChild);

        if(parentNode->height >= parentNode->rightChild->height+1 && BFisOkay(parentNode)){
            return;
        }


    }
    if(parentNode->nodeID==requestedNodeID){
        parentNode->height=getNodeHeight(parentNode->rightChild,parentNode->leftChild);
        parentNode->subTreeNodeNumber=getSubtreeNodeNumber(parentNode->rightChild,parentNode->leftChild);
        return;
    }


    int leftHeight=sideHeight(parentNode->leftChild);

    int rightHeight = sideHeight(parentNode->rightChild);
    if(leftHeight-rightHeight==2){
        int subLeftHeight;
        int subRightHeight;

        subLeftHeight=sideHeight(parentNode->leftChild->leftChild);

        subRightHeight=sideHeight(parentNode->leftChild->rightChild);

        if((subLeftHeight-subRightHeight)==1||(subLeftHeight-subRightHeight)==0){
            correctionLL(parentNode);
            return;
        }//LL

        if((subLeftHeight-subRightHeight)==-1){
            correctionLR(parentNode);
            return;
        }//LR
    }
    if(leftHeight-rightHeight==-2){
        int subLeftHeight;
        int subRightHeight;

        subRightHeight=sideHeight(parentNode->rightChild->rightChild);

        subLeftHeight=sideHeight(parentNode->rightChild->leftChild);

        if((subLeftHeight-subRightHeight)==1){
            correctionRL(parentNode);
            return;
        }//RL
        if((subLeftHeight-subRightHeight)==-1 ||(subLeftHeight-subRightHeight)==0){
            correctionRR(parentNode);
            return;
        }//RR

        throw;
    }


    parentNode->height=getNodeHeight(parentNode->leftChild,parentNode->leftChild);
    parentNode->subTreeNodeNumber=getSubtreeNodeNumber(parentNode->rightChild,parentNode->leftChild);
}



template <class T, class ID>
sideHeightInt AVLTree<T,ID>::node::sideHeight(node* childNode){
    if(childNode==NULL) {
        return 0;
    }else return childNode->height+1;
}

template <class T, class ID>
void AVLTree<T,ID>::node::swapNodes(AVLTree::node *fNode, AVLTree::node *sNode) {//DONT SWAP CHILDREN!
    node temporary;
    temporary.subTreeNodeNumber=fNode->subTreeNodeNumber;
    temporary.height=fNode->height;
    temporary.nodeID=fNode->nodeID;
    temporary._data=fNode->_data;

    fNode->subTreeNodeNumber=sNode->subTreeNodeNumber;
    fNode->height=sNode->height;
    fNode->nodeID=sNode->nodeID;
    fNode->_data=sNode->_data;

    sNode->subTreeNodeNumber=fNode->subTreeNodeNumber;
    sNode->height=temporary.height;
    sNode->nodeID=temporary.nodeID;
    sNode->_data=temporary._data;
}

template <class T, class ID>
void AVLTree<T,ID>::node::correctionLL(AVLTree::node *parentNode){
    node* lChild=parentNode->leftChild;
    swapNodes(parentNode,lChild);

    parentNode->leftChild=parentNode->leftChild->leftChild;
    if( parentNode->leftChild!=NULL) parentNode->leftChild->parent=parentNode;

    lChild->leftChild=lChild->rightChild;
    if( lChild->leftChild!=NULL) lChild->leftChild->parent=lChild;

    lChild->rightChild=parentNode->rightChild;
    if( lChild->rightChild!=NULL) lChild->rightChild->parent=lChild;

    parentNode->rightChild=lChild;
    if( parentNode->rightChild!=NULL) parentNode->rightChild->parent=parentNode;

    lChild->subTreeNodeNumber=getSubtreeNodeNumber(lChild->rightChild,lChild->leftChild);
    lChild->height=getNodeHeight(lChild->rightChild,lChild->leftChild);
    parentNode->subTreeNodeNumber=getSubtreeNodeNumber(parentNode->rightChild,parentNode->leftChild);
    parentNode->height=getNodeHeight(parentNode->rightChild,parentNode->leftChild);
}
template <class T, class ID>
void AVLTree<T,ID>::node::correctionRR(AVLTree::node *parentNode){
    node* rChild=parentNode->rightChild;
    swapNodes(parentNode,rChild);

    parentNode->rightChild=parentNode->rightChild->rightChild;
    parentNode->rightChild->parent=parentNode;

    rChild->rightChild=rChild->leftChild;
    if( rChild->rightChild!=NULL) rChild->rightChild->parent=rChild;

    rChild->leftChild=parentNode->leftChild;
    if( rChild->leftChild!=NULL) rChild->leftChild->parent=rChild;

    parentNode->leftChild=rChild;
    if( parentNode->leftChild!=NULL) parentNode->leftChild->parent=parentNode;

    rChild->height=getNodeHeight(rChild->rightChild,rChild->leftChild);
    rChild->subTreeNodeNumber=getSubtreeNodeNumber(rChild->rightChild,rChild->leftChild);
    parentNode->height=getNodeHeight(parentNode->rightChild,parentNode->leftChild);
    parentNode->subTreeNodeNumber=getSubtreeNodeNumber(parentNode->rightChild,parentNode->leftChild);
}

template <class T, class ID>
void AVLTree<T,ID>::node::correctionLR(AVLTree::node *parentNode){
    node* lrChild=parentNode->leftChild->rightChild;
    swapNodes(parentNode,lrChild);

    parentNode->leftChild->rightChild=lrChild->leftChild;
    if( parentNode->leftChild->rightChild!=NULL) parentNode->leftChild->rightChild->parent=parentNode->leftChild;

    lrChild->leftChild=lrChild->rightChild;
    if( lrChild->leftChild!=NULL) lrChild->leftChild->parent=lrChild;

    lrChild->rightChild=parentNode->rightChild;
    if( lrChild->rightChild!=NULL) lrChild->rightChild->parent=lrChild;

    parentNode->rightChild=lrChild;
    if( parentNode->rightChild!=NULL) parentNode->rightChild->parent=parentNode;

    lrChild->subTreeNodeNumber=getSubtreeNodeNumber(lrChild->rightChild,lrChild->leftChild);
    lrChild->height=getNodeHeight(lrChild->rightChild,lrChild->leftChild);
    parentNode->leftChild->height=getNodeHeight(parentNode->leftChild->rightChild,parentNode->leftChild->leftChild);
    parentNode->leftChild->subTreeNodeNumber=getSubtreeNodeNumber(parentNode->leftChild->rightChild,parentNode->leftChild->leftChild);
    parentNode->height=getNodeHeight(parentNode->rightChild,parentNode->leftChild);
    parentNode->subTreeNodeNumber=getSubtreeNodeNumber(parentNode->rightChild,parentNode->leftChild);
}

template <class T, class ID>
void AVLTree<T,ID>::node::correctionRL(AVLTree::node *parentNode){
    node* rlChild=parentNode->rightChild->leftChild;
    swapNodes(parentNode,rlChild);


    parentNode->rightChild->leftChild=rlChild->rightChild;
    if( parentNode->rightChild->leftChild!=NULL) parentNode->rightChild->leftChild->parent=parentNode->rightChild;

    rlChild->rightChild=rlChild->leftChild;
    if( rlChild->rightChild!=NULL) rlChild->rightChild->parent= rlChild;

    rlChild->leftChild=parentNode->leftChild;
    if( rlChild->leftChild!=NULL) rlChild->leftChild->parent=rlChild;

    parentNode->leftChild=rlChild;
    if( parentNode->leftChild!=NULL)  parentNode->leftChild->parent=parentNode;

    rlChild->subTreeNodeNumber=getSubtreeNodeNumber(rlChild->rightChild,rlChild->leftChild);
    rlChild->height=getNodeHeight(rlChild->rightChild,rlChild->leftChild);
    parentNode->rightChild->height=getNodeHeight(parentNode->leftChild->rightChild,parentNode->leftChild->leftChild);
    parentNode->rightChild->subTreeNodeNumber=getSubtreeNodeNumber(parentNode->leftChild->rightChild,parentNode->leftChild->leftChild);
    parentNode->height=getNodeHeight(parentNode->rightChild,parentNode->leftChild);
    parentNode->subTreeNodeNumber=getSubtreeNodeNumber(parentNode->rightChild,parentNode->leftChild);

}

template <class T, class ID>
bool AVLTree<T,ID>::node::BFisOkay(AVLTree::node *parentNode)
{
    int l=sideHeight(parentNode->leftChild);
    int r=sideHeight(parentNode->rightChild);
    return l - r < 2 && l - r > -2;
}

template <class T, class ID>
void AVLTree<T,ID>::node::findSmallestNode(AVLTree* tree) {
    node* nodeIterator=tree->treeEntry;
    if((tree->treeEntry)!=NULL){
        while(nodeIterator->leftChild!=NULL){
            nodeIterator=nodeIterator->leftChild;
        }
    }else nodeIterator= nullptr;
    tree->smallestNode = nodeIterator;
}




#endif //INC_1W_DS_AVL_H
