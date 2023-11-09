#include "SelectionTree.h"

bool SelectionTree::Insert(LoanBookData* newData) {

    int treeHeight = 4;
    
    if(root == NULL) {

        SelectionTreeNode* newSelNode = new SelectionTreeNode();
        root = newSelNode;
        createSelTree(root, treeHeight);
    }

    SelectionTreeNode* pCur = root;
    for (int i = 2; i >= 0; i--) {

        int direction = (newData->getCode() >> (2 * i)) & 0x3;

        if (direction == 0 && pCur->getLeftChild() != NULL) 
            pCur = pCur->getLeftChild();

        else if (direction == 1 && pCur->getRightChild() != NULL) 
            pCur = pCur->getRightChild();

        else 
            return false;
    }

    // pCur이 가리키는 min heap에 newData 삽입
    // heap 정렬을 통해 수정된 root 값을 pCur에서 세팅하도록 설정

    while(pCur != root) {

        LoanBookData* childData = new LoanBookData;
        LoanBookData* temp = pCur->getBookData();
        childData->setBookData(temp->getName(), temp->getCode(), temp->getAuthor(), temp->getYear(), temp->getLoanCount());
        
        pCur = pCur->getParent();
        if((pCur->getBookData() == NULL) || (childData->getName() < pCur->getBookData()->getName()))
            pCur->setBookData(childData);
    }
}

bool SelectionTree::Delete() {

}

bool SelectionTree::printBookData(int bookCode) {

}

void SelectionTree::createSelTree(SelectionTreeNode* root, int treeHeight) {

    if (treeHeight == 0) 
        return; 
    
    SelectionTreeNode* newLeftSelNode = new SelectionTreeNode();
    root->setLeftChild(newLeftSelNode);
    newLeftSelNode->setParent(root);

    SelectionTreeNode* newRightSelNode = new SelectionTreeNode();
    root->setRightChild(newRightSelNode);
    newRightSelNode->setParent(root);
    
    createSelTree(root->getLeftChild(), treeHeight-1);
    createSelTree(root->getRightChild(), treeHeight-1);
}