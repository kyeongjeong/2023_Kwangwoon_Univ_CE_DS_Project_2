#include "LoanBookHeap.h"

void LoanBookHeap::heapifyUp(LoanBookHeapNode* pN) {

    LoanBookHeapNode* pCur = pN;
    LoanBookHeapNode* pPar = pCur->getParent();

    while ((pCur->getParent() != NULL) && (pCur->getBookData()->getName() < pPar->getBookData()->getName())) {
        
        LoanBookData* temp = pCur->getBookData();
        pCur->setBookData(pPar->getBookData());
        pPar->setBookData(temp);

        pCur = pPar;
    }
}

void LoanBookHeap::heapifyDown(LoanBookHeapNode* pN) {

}

bool LoanBookHeap::Insert(LoanBookData* data) {
    
    LoanBookHeapNode *newHeapNode = new LoanBookHeapNode;
    newHeapNode->setBookData(data);
    
    if(root == NULL) {
        root = newHeapNode;
        que.push(root);
        return true;
    }

    LoanBookHeapNode* pCur = que.front();
    if(pCur->getLeftChild() == NULL) {
        pCur->setLeftChild(newHeapNode);
        que.push(newHeapNode);
    }
    else if(pCur->getRightChild() == NULL) {
        pCur->setRightChild(newHeapNode);
        que.push(newHeapNode);
    }

    if((pCur->getLeftChild() != NULL) && (pCur->getRightChild() != NULL)) 
        que.pop();

    if(pCur->getBookData()->getName() < pCur->getParent()->getBookData()->getName())
        heapifyUp(pCur);
    
    return true;
}