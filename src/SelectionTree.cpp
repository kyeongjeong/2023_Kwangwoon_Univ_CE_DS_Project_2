#include "SelectionTree.h"

bool SelectionTree::Insert(LoanBookData* newData) {

    int treeHeight = 4;
    
    if(root == NULL) {

        SelectionTreeNode* newSelNode = new SelectionTreeNode();
        root = newSelNode;
        createSelTree(root, treeHeight-1);
    }

    int direction[3] = {((newData->getCode() / 100) / 4), (((newData->getCode() / 100) % 4) / 2), (((newData->getCode() / 100) % 4) % 2)};
    SelectionTreeNode* pCur = root;
    for (int i = 0; i < 3; i++) {

        if (direction[i] == 0 && pCur->getLeftChild() != NULL) 
            pCur = pCur->getLeftChild();

        else if (direction[i] == 1 && pCur->getRightChild() != NULL) 
            pCur = pCur->getRightChild();

        else 
            return false;
    }

    pCur->getHeap()->Insert(newData);
    pCur->setBookData(pCur->getHeap()->getRoot()->getBookData());

    while(pCur != root) {

        LoanBookData* childData = new LoanBookData;
        LoanBookData* temp = pCur->getBookData();
        childData->setBookData(temp->getName(), temp->getCode(), temp->getAuthor(), temp->getYear(), temp->getLoanCount());
        
        pCur = pCur->getParent();
        if((pCur->getBookData() == NULL) || (childData->getName() < pCur->getBookData()->getName()))
            pCur->setBookData(childData);
    }
    // cout << root->getBookData()->getName() << endl;
}

bool SelectionTree::Delete() {

    int tempNum = root->getBookData()->getCode();
    int direction[3] = {((root->getBookData()->getCode() / 100) / 4), (((root->getBookData()->getCode() / 100) % 4) / 2), (((root->getBookData()->getCode() / 100) % 4) % 2)};
    SelectionTreeNode* pCur = root;
    for (int i = 0; i < 3; i++) {

        pCur->setBookData(NULL);
        if (direction[i] == 0 && pCur->getLeftChild() != NULL) 
            pCur = pCur->getLeftChild();

        else if (direction[i] == 1 && pCur->getRightChild() != NULL) 
            pCur = pCur->getRightChild();

        else 
            return false;
    }

    LoanBookHeap* tempHeap = pCur->getHeap();
    tempHeap->heapifyDown(tempHeap->getRoot());
    pCur->setBookData(tempHeap->getRoot()->getBookData());

    while(pCur->getParent() != NULL) {

        pCur = pCur->getParent();
        LoanBookData* leftData = pCur->getLeftChild()->getBookData();
        LoanBookData* rightData = pCur->getRightChild()->getBookData();

        if((leftData == NULL) && (rightData == NULL))
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
    // if(root->getBookData() != NULL)
    //     cout << root->getBookData()->getName() << endl;
    return true;
}

bool SelectionTree::printBookData(int bookCode) {

    int direction[3] = {((bookCode / 100) / 4), (((bookCode / 100) % 4) / 2), (((bookCode / 100) % 4) % 2)};
    SelectionTreeNode* pCur = root;

    for (int i = 0; i < 3; i++) {

        if (direction[i] == 0 && pCur->getLeftChild() != NULL) 
            pCur = pCur->getLeftChild();

        else if (direction[i] == 1 && pCur->getRightChild() != NULL) 
            pCur = pCur->getRightChild();

        else 
            return false;
    }

    if(pCur->getHeap()->getRoot() == NULL)
        return false;

    pCur->getDataMap().clear();
    traversalHeap(pCur->getHeap()->getRoot(), pCur);
    map<string, LoanBookData*> pMap = pCur->getDataMap();

    *fout << "========PRiNT_ST========" << endl;
    map<string, LoanBookData*>::iterator mIter;
    for(mIter = pMap.begin(); mIter != pMap.end(); mIter++) {
        *fout << mIter->second->getName() << "/" << mIter->second->getCode();
        if(mIter->second->getCode() == 0)
			*fout << "00";
        *fout << "/" << mIter->second->getAuthor() << "/" << mIter->second->getYear() << "/" << mIter->second->getLoanCount() << endl;
    }
    *fout << "========================" << endl << endl;

    return true;
}

void SelectionTree::createSelTree(SelectionTreeNode* pCur, int treeHeight) {

    if (treeHeight == 0) {
        LoanBookHeap* newHeap = new LoanBookHeap;
        pCur->setHeap(newHeap);
        return; 
    }
    
    SelectionTreeNode* newLeftSelNode = new SelectionTreeNode();
    pCur->setLeftChild(newLeftSelNode);
    newLeftSelNode->setParent(pCur);

    SelectionTreeNode* newRightSelNode = new SelectionTreeNode();
    pCur->setRightChild(newRightSelNode);
    newRightSelNode->setParent(pCur);
    
    createSelTree(pCur->getLeftChild(), treeHeight-1);
    createSelTree(pCur->getRightChild(), treeHeight-1);
}

LoanBookHeapNode* SelectionTree::traversalHeap(LoanBookHeapNode* pHeap, SelectionTreeNode* pCur) {

    if (pHeap == NULL) 
        return NULL;

    traversalHeap(pHeap->getLeftChild(), pCur);
    if(pHeap != NULL)
        pCur->setDataMap(pHeap->getBookData()->getName(), pHeap->getBookData());
    traversalHeap(pHeap->getRightChild(), pCur);

    return pHeap;
}