#include "BpTree.h"
#include <cmath>

bool BpTree::Insert(LoanBookData* newData) {

	if(root == NULL) {

		BpTreeDataNode* dataNode = new BpTreeDataNode();
		dataNode->insertDataMap(newData->getName(), newData);
		root = dataNode;
		return true;
	}

	else if(root->getMostLeftChild() == NULL) {

		root->insertDataMap(newData->getName(), newData);
		if(excessDataNode(root))
			splitDataNode(root);
		return true;
	}

	else {

		BpTreeNode* pCur = searchDataNode(newData->getName());
		pCur->insertDataMap(newData->getName(), newData);
	
		if(excessDataNode(pCur))
			splitDataNode(pCur);
		
		pCur = pCur->getParent();
		while(pCur != NULL){
			if(excessIndexNode(pCur)){ 
				splitIndexNode(pCur);
				pCur = pCur->getParent();
			}
			else
				break;
		}
	}	
	return true;
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
	if (pDataNode->getDataMap()->size() > order - 1) return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode->getIndexMap()->size() > order - 1) return true;//order is equal to the number of elements 
	else return false;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
	
	int splitKey = ceil((order-1)/2.0);
	int count = 0;
	BpTreeDataNode* newDataNode = new BpTreeDataNode();
	
	map <string, LoanBookData*>::iterator mIter = pDataNode->getDataMap()->begin();
	for(int i = 1; i < splitKey; i++) {

		newDataNode->insertDataMap(mIter->first, mIter->second);
		pDataNode->deleteMap(mIter->first);
		mIter++;
	}
	string newKey = mIter->first;

	if(pDataNode->getParent() == NULL) {

		BpTreeIndexNode *newIndexNode = new BpTreeIndexNode();
		newIndexNode->insertIndexMap(newKey, pDataNode);	
		newIndexNode->setMostLeftChild(newDataNode);
		root = newIndexNode;

		newDataNode->setParent(newIndexNode);
		pDataNode->setParent(newIndexNode);
	}
	else {

		BpTreeNode *indexNode = pDataNode->getParent();
		map<string, BpTreeNode *> *indexMap = indexNode->getIndexMap();

		indexNode->insertIndexMap(newKey, pDataNode);
		
		// iIter = findIndexMap->begin();
		// if (frontNode->getDataMap()->begin()->first < iIter->first)
		// 	pDataNode->getParent()->setMostLeftChild(frontNode);

		// for (; iIter != findIndexMap->end(); iIter++)
		// {
		// 	if (frontNode->getDataMap()->begin()->first == iIter->first)
		// 		iIter->second = frontNode;

		// 	if (newNum == iIter->first)
		// 		iIter->second = backNode;
		// }
		// frontNode->setParent(pDataNode->getParent());
		// backNode->setParent(pDataNode->getParent());

		// // if indexNode's node count is bigger than order
		// if (pDataNode->getParent()->getIndexMap()->size() > (order - 1))
		// 	splitIndexNode(pDataNode->getParent());

		// pDataNode->getPrev()->setNext(frontNode);
		// frontNode->setPrev(pDataNode->getPrev());

	}
	newDataNode->setNext(pDataNode);
	pDataNode->setPrev(newDataNode);
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	

}

BpTreeNode* BpTree::searchDataNode(string name) {
	
	BpTreeNode* pCur = root;
	BpTreeNode* pNext;

	while(pCur->getMostLeftChild() != NULL) 
		pCur = pCur->getMostLeftChild();
	pNext = pCur;

	while(pCur != NULL) {

		if(pCur->getNext() == NULL)
			return pCur;
		
		pNext = pCur->getNext();
		map <string, LoanBookData*>::iterator mIter = pNext->getDataMap()->begin();
		if(name < mIter->first)
			return pCur;
		
		pCur = pCur->getNext();
	}	
	return pCur;
}

bool BpTree::searchBook(string name) {

}

bool BpTree::searchRange(string start, string end) {
	
}
