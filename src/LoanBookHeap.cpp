#include "LoanBookHeap.h"

void LoanBookHeap::heapifyUp(LoanBookHeapNode* pN) {

    LoanBookHeapNode* pCur = pN;
    LoanBookHeapNode* pPar;

    while (pCur->getParent() != NULL) {
        
        pPar = pCur->getParent();
        if(pCur->getBookData()->getName() < pPar->getBookData()->getName()) {

            LoanBookData* temp = new LoanBookData;
            temp->setBookData(pCur->getBookData()->getName(), pCur->getBookData()->getCode(), pCur->getBookData()->getAuthor(), pCur->getBookData()->getYear(), pCur->getBookData()->getLoanCount());
            pCur->setBookData(pPar->getBookData());
            pPar->setBookData(temp);
        }
        pCur = pPar;
    }
}

void LoanBookHeap::heapifyDown(LoanBookHeapNode* pN) {

    if((pN->getLeftChild() == NULL) || (pN->getRightChild() == NULL)) {
        pN->setBookData(NULL);
        return;
    }
    
    LoanBookHeapNode* pCur;
    queue<LoanBookHeapNode*> que;
    que.push(pN);

    while (!que.empty()) {

        pCur = que.front();
        que.pop();

        if (pCur->getLeftChild() != NULL) 
            que.push(pCur->getLeftChild());
        
        if (pCur->getRightChild() != NULL) 
            que.push(pCur->getRightChild());

        if (que.empty()) 
            break;
    }

    LoanBookData* temp = new LoanBookData;
    temp->setBookData(pCur->getBookData()->getName(), pCur->getBookData()->getCode(), pCur->getBookData()->getAuthor(), pCur->getBookData()->getYear(), pCur->getBookData()->getLoanCount());
    pN->setBookData(temp);

    LoanBookHeapNode* pPar = pCur->getParent();
    if(pPar->getLeftChild() == pCur)
        pPar->setLeftChild(NULL);
    else if(pPar->getRightChild() == pCur)
        pPar->setRightChild(NULL);

    delete pCur;
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
        pCur->getLeftChild()->setParent(pCur);
        pCur = pCur->getLeftChild();
        que.push(newHeapNode);
    }
    else if(pCur->getRightChild() == NULL) {
        pCur->setRightChild(newHeapNode);
        pCur->getRightChild()->setParent(pCur);
        pCur = pCur->getRightChild();
        que.push(newHeapNode);
    }

    if((pCur->getParent()->getLeftChild() != NULL) && (pCur->getParent()->getRightChild() != NULL)) 
        que.pop();

    if(pCur->getBookData()->getName() < pCur->getParent()->getBookData()->getName())
        heapifyUp(pCur);
            
    return true;
}