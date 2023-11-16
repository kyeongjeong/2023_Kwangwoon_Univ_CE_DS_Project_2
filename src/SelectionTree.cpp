#include "SelectionTree.h"

bool SelectionTree::Insert(LoanBookData* newData) {

    int treeHeight = 4; // Set the height of the Selection Tree
    if (root == NULL) {
        SelectionTreeNode* newSelNode = new SelectionTreeNode(); // Create a new Selection Tree node
        root = newSelNode; // Set the new node as the root
        createSelTree(root, treeHeight - 1); // Create the Selection Tree structure based on the specified height
    }

    int direction[3] = {((newData->getCode() / 100) / 4), (((newData->getCode() / 100) % 4) / 2), (((newData->getCode() / 100) % 4) % 2)};
    SelectionTreeNode* pCur = root;
    for (int i = 0; i < 3; i++) { // Traverse the Selection Tree based on the code of the new loan book data

        if (direction[i] == 0 && pCur->getLeftChild() != NULL) 
            pCur = pCur->getLeftChild();

        else if (direction[i] == 1 && pCur->getRightChild() != NULL) 
            pCur = pCur->getRightChild();

        else 
            return false; // Return false if the specified direction is invalid
    }

    pCur->getHeap()->Insert(newData); // Insert the new loan book data into the heap of the current node
    pCur->setBookData(pCur->getHeap()->getRoot()->getBookData()); // Update the book data of the current node

    while(pCur != root) {

        LoanBookData* childData = new LoanBookData;
        LoanBookData* temp = pCur->getBookData();
        childData->setBookData(temp->getName(), temp->getCode(), temp->getAuthor(), temp->getYear(), temp->getLoanCount());
        
        pCur = pCur->getParent();
        if((pCur->getBookData() == NULL) || (childData->getName() < pCur->getBookData()->getName()))
            pCur->setBookData(childData); // Update the book data of the parent node with the smaller name or if it is null
    }
    return true; // Return true to indicate successful insertion
}

bool SelectionTree::Delete() {

    //
    string tempStr = root->getBookData()->getName();
    // Determine the traversal direction based on the code of the root node
    int direction[3] = {((root->getBookData()->getCode() / 100) / 4), (((root->getBookData()->getCode() / 100) % 4) / 2), (((root->getBookData()->getCode() / 100) % 4) % 2)};
    SelectionTreeNode* pCur = root;
    for (int i = 0; i < 3; i++) { // Traverse the Selection Tree based on the determined direction

        pCur->setBookData(NULL); // Set the book data of the current node to NULL
        if (direction[i] == 0 && pCur->getLeftChild() != NULL) 
            pCur = pCur->getLeftChild(); 

        else if (direction[i] == 1 && pCur->getRightChild() != NULL) 
            pCur = pCur->getRightChild();

        else 
            return false; // Return false if the specified direction is invalid
    }

    pCur->getHeap()->heapifyDown(pCur->getHeap()->getRoot()); // Perform heapify-down operation after deletion
    pCur->setBookData(pCur->getHeap()->getRoot()->getBookData()); // Update the book data of the current node

    while(pCur->getParent() != NULL) { // Update the book data of parent nodes along the path to the root

        pCur = pCur->getParent();
        LoanBookData* leftData = pCur->getLeftChild()->getBookData();
        LoanBookData* rightData = pCur->getRightChild()->getBookData();

        if((leftData == NULL) && (rightData == NULL)) // Check and update the book data of the current node based on its children's data
            continue;

        else if((rightData == NULL) || ((leftData != NULL) && (leftData->getName() < rightData->getName()))) {
            LoanBookData* tempData = new LoanBookData;
            tempData->setBookData(leftData->getName(), leftData->getCode(), leftData->getAuthor(), leftData->getYear(), leftData->getLoanCount());
            pCur->setBookData(tempData);
        }

        else {
            LoanBookData* tempData = new LoanBookData;
            tempData->setBookData(rightData->getName(), rightData->getCode(), rightData->getAuthor(), rightData->getYear(), rightData->getLoanCount());
            pCur->setBookData(tempData);
        }
    }
    //
    if(root->getBookData() != NULL)
        cout << tempStr << " delete -> new root: "<< root->getBookData()->getName() << endl;
    return true;
}

bool SelectionTree::printBookData(int bookCode) {

    // Determine the traversal direction based on the code of the book
    int direction[3] = {((bookCode / 100) / 4), (((bookCode / 100) % 4) / 2), (((bookCode / 100) % 4) % 2)};
    SelectionTreeNode* pCur = root;

    // Traverse the Selection Tree based on the determined direction
    for (int i = 0; i < 3; i++) {

        if (direction[i] == 0 && pCur->getLeftChild() != NULL) 
            pCur = pCur->getLeftChild();

        else if (direction[i] == 1 && pCur->getRightChild() != NULL) 
            pCur = pCur->getRightChild();

        else 
            return false; // Return false if the specified direction is invalid
    }

    if (pCur->getHeap()->getRoot() == NULL)
        return false;

    pCur->getDataMap().clear(); // Clear the data map of the current node
    traversalHeap(pCur->getHeap()->getRoot(), pCur); // Traverse the heap and populate the data map
    map<string, LoanBookData*> pMap = pCur->getDataMap(); // Retrieve the populated data map

    *fout << "========PRINT_ST========" << endl;
    map<string, LoanBookData*>::iterator mIter;
    for(mIter = pMap.begin(); mIter != pMap.end(); mIter++) { // Print the book data from the data map
        *fout << mIter->second->getName() << "/" << mIter->second->getCode();
        if(mIter->second->getCode() == 0)
			*fout << "00";
        *fout << "/" << mIter->second->getAuthor() << "/" << mIter->second->getYear() << "/" << mIter->second->getLoanCount() << endl;
    }
    *fout << "========================" << endl << endl;

    return true; // Return true to indicate successful printing
}

void SelectionTree::createSelTree(SelectionTreeNode* pCur, int treeHeight) {

    // Base case: If the tree height is 0, create a new heap for the current node
    if (treeHeight == 0) {
        LoanBookHeap* newHeap = new LoanBookHeap;
        pCur->setHeap(newHeap);
        return;
    }
    
    // Create left and right child nodes for the current node
    SelectionTreeNode* newLeftSelNode = new SelectionTreeNode();
    pCur->setLeftChild(newLeftSelNode);
    newLeftSelNode->setParent(pCur);

    SelectionTreeNode* newRightSelNode = new SelectionTreeNode();
    pCur->setRightChild(newRightSelNode);
    newRightSelNode->setParent(pCur);
    
    // Recursively create the Selection Tree for the left and right children
    createSelTree(pCur->getLeftChild(), treeHeight-1);
    createSelTree(pCur->getRightChild(), treeHeight-1);
}

LoanBookHeapNode* SelectionTree::traversalHeap(LoanBookHeapNode* pHeap, SelectionTreeNode* pCur) {

    if (pHeap == NULL) // Base case: If the current heap node is NULL, return NULL
        return NULL;

    traversalHeap(pHeap->getLeftChild(), pCur); // Traverse the left subtree of the heap  
    if(pHeap != NULL) // Populate the data map of the current SelectionTreeNode with book data from the heap
        pCur->setDataMap(pHeap->getBookData()->getName(), pHeap->getBookData());    
    traversalHeap(pHeap->getRightChild(), pCur); // Traverse the right subtree of the heap

    return pHeap;
}