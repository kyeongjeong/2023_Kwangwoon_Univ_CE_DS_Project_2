#include "LoanBookHeap.h"

void LoanBookHeap::heapifyUp(LoanBookHeapNode* pN) {

}

void LoanBookHeap::heapifyDown(LoanBookHeapNode* pN) {

}

bool LoanBookHeap::Insert(LoanBookData* data) {
    
    if(root == NULL) {

        LoanBookHeapNode *newHeapNode = new LoanBookHeapNode;
        newHeapNode->setBookData(data);
        root = newHeapNode;
        return true;
    }

    
}