#pragma once
#include "LoanBookData.h"
#include "LoanBookHeapNode.h"
#include <queue>

class LoanBookHeap
{
private:
    LoanBookHeapNode* root;
    queue<LoanBookHeapNode*> que;

public:
    LoanBookHeap() {
        this->root = NULL;
    }
    ~LoanBookHeap() {

    }
    
    void setRoot(LoanBookHeapNode* pN) { this->root = pN; }
    LoanBookHeapNode* getRoot() { return root; }

    void heapifyUp(LoanBookHeapNode* pN);
    void heapifyDown(LoanBookHeapNode* pN);
    
    bool Insert(LoanBookData* data);
};