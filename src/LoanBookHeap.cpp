#include "LoanBookHeap.h"

void LoanBookHeap::heapifyUp(LoanBookHeapNode* pN) {

    LoanBookHeapNode* pCur = pN;
    LoanBookHeapNode* pPar;

    while (pCur->getParent() != NULL) { // Continue heapify-up until the current node reaches the root
        
        pPar = pCur->getParent();  // Get the parent node

        // If the book data of the current node is smaller than the parent's book data, swap them
        if(pCur->getBookData()->getName() < pPar->getBookData()->getName()) {

            LoanBookData* temp = new LoanBookData; // Swap the book data between the current node and the parent node
            temp->setBookData(pCur->getBookData()->getName(), pCur->getBookData()->getCode(), pCur->getBookData()->getAuthor(), pCur->getBookData()->getYear(), pCur->getBookData()->getLoanCount());
            pCur->setBookData(pPar->getBookData());
            pPar->setBookData(temp);
        }
        pCur = pPar;  // Move to the parent node for the next iteration
    }
}

void LoanBookHeap::heapifyDown(LoanBookHeapNode* pN) {

    // If the node has no left or right child, set its book data to NULL and return
    if((pN->getLeftChild() == NULL) && (pN->getRightChild() == NULL)) {
        pN->setBookData(NULL);
        return;
    }
    
    LoanBookHeapNode* pCur;  // Current node
    queue<LoanBookHeapNode*> que;  // Queue for level order traversal
    que.push(pN);  // Start with the given node

    while (!que.empty()) { // Perform level order traversal to find the last node in the heap

        pCur = que.front();
        que.pop();

        if (pCur->getLeftChild() != NULL)  // Enqueue left child if it exists
            que.push(pCur->getLeftChild()); 
       
        if (pCur->getRightChild() != NULL)  // Enqueue right child if it exists
            que.push(pCur->getRightChild());
       
        if (que.empty())  // If the queue is empty, break from the loop
            break;
    }

    LoanBookData* temp = new LoanBookData; // Create a temporary LoanBookData to store the last node's book data
    temp->setBookData(pCur->getBookData()->getName(), pCur->getBookData()->getCode(), pCur->getBookData()->getAuthor(), pCur->getBookData()->getYear(), pCur->getBookData()->getLoanCount());
    pN->setBookData(temp);  // Set the book data of the current node to the temporary data
    
    LoanBookHeapNode* pPar = pCur->getParent(); // Get the parent of the last node  
    if(pPar->getLeftChild() == pCur) // Update the parent's child pointer based on the relationship with the last node
        pPar->setLeftChild(NULL);
    else if(pPar->getRightChild() == pCur)
        pPar->setRightChild(NULL);

    delete pCur; // Delete the last node to maintain the heap structure
}

bool LoanBookHeap::Insert(LoanBookData* data) {
    
    LoanBookHeapNode *newHeapNode = new LoanBookHeapNode; // Create a new heap node and set its book data
    newHeapNode->setBookData(data);
    
    if(root == NULL) {  // If the heap is empty, set the new node as the root and enqueue it
        root = newHeapNode;
        que.push(root);
        return true;
    }

    LoanBookHeapNode* pCur = que.front(); // Get the front node from the queue
    if(pCur->getLeftChild() == NULL) { // If the left child of the current node is empty, insert the new node as the left child
        pCur->setLeftChild(newHeapNode);
        pCur->getLeftChild()->setParent(pCur);
        pCur = pCur->getLeftChild();
        que.push(newHeapNode);
    }
    else if(pCur->getRightChild() == NULL) { // If the right child of the current node is empty, insert the new node as the right child
        pCur->setRightChild(newHeapNode);
        pCur->getRightChild()->setParent(pCur);
        pCur = pCur->getRightChild();
        que.push(newHeapNode);
    }

    // If both left and right children are present, dequeue the front node
    if((pCur->getParent()->getLeftChild() != NULL) && (pCur->getParent()->getRightChild() != NULL)) 
        que.pop();

    // Perform heapify-up operation to maintain the heap property
    if(pCur->getBookData()->getName() < pCur->getParent()->getBookData()->getName())
        heapifyUp(pCur);
            
    return true;
}