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
	for(int i = 0; i < splitKey; i++) {

		newDataNode->insertDataMap(mIter->first, mIter->second);
		map <string, LoanBookData*>::iterator mIter2 = mIter;
		mIter++;
		pDataNode->deleteMap(mIter2->first);
	}
	string pKey = mIter->first;

	if(pDataNode->getParent() == NULL) {

		BpTreeIndexNode *newIndexNode = new BpTreeIndexNode();
		root = newIndexNode;
		newIndexNode->insertIndexMap(pKey, pDataNode);	
		newIndexNode->setMostLeftChild(newDataNode);

		newDataNode->setParent(newIndexNode);
		pDataNode->setParent(newIndexNode);
	}
	else {

		BpTreeNode *indexNode = pDataNode->getParent();
		map<string, BpTreeNode *> *indexMap = indexNode->getIndexMap();

		indexNode->insertIndexMap(pKey, pDataNode);
		
		map<string, BpTreeNode *>::iterator iIter = indexMap->begin();
		if (newDataNode->getDataMap()->begin()->first < iIter->first)
			indexNode->setMostLeftChild(newDataNode);

		for (; iIter != indexMap->end(); iIter++) { 
	 	
			if (newDataNode->getDataMap()->begin()->first == iIter->first)
		 		iIter->second = newDataNode;

		 	if (pKey == iIter->first)
		 		iIter->second = pDataNode;
		}	
		newDataNode->setParent(indexNode);
		pDataNode->setParent(indexNode);

		if (indexMap->size() > (order - 1))
		 	splitIndexNode(indexNode);

		pDataNode->getPrev()->setNext(newDataNode);
		newDataNode->setPrev(pDataNode->getPrev());
	}
	newDataNode->setNext(pDataNode);
	pDataNode->setPrev(newDataNode);
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	
	int splitKey = ceil((order-1)/2.0);
	int count = 0;
	BpTreeIndexNode* newIndexNode = new BpTreeIndexNode();
	
	map <string, BpTreeNode*>::iterator iIter = pIndexNode->getIndexMap()->begin();
	for(int i = 0; i < splitKey; i++) {

		newIndexNode->insertIndexMap(iIter->first, iIter->second);
		pIndexNode->deleteMap(iIter->first);
		iIter++;
	}
	string pKey = iIter->first;

	if(pIndexNode->getParent() == NULL) {

		BpTreeNode *upIndexNode = new BpTreeIndexNode();
		root = upIndexNode;
		upIndexNode->insertIndexMap(pKey, pIndexNode);	
		upIndexNode->setMostLeftChild(newIndexNode);

		newIndexNode->setParent(upIndexNode);
		pIndexNode->setParent(upIndexNode);
		newIndexNode->setMostLeftChild(pIndexNode->getMostLeftChild());
	}
	else {

		BpTreeNode *upIndexNode = pIndexNode->getParent();
		upIndexNode->insertIndexMap(pKey, pIndexNode);

		map<string, BpTreeNode *> upIndexMap = *(upIndexNode->getIndexMap());
		string nKey = newIndexNode->getIndexMap()->begin()->first;

		iIter = upIndexMap.begin();
		if(nKey < iIter->first)
			upIndexNode->setMostLeftChild(newIndexNode);

		for(; iIter != upIndexMap.end(); iIter++) {

			if (iIter == upIndexMap.end()) {
				iIter->second = pIndexNode;
				break;
			}

			++iIter;
			if (nKey < iIter->first) {
				(--iIter)->second = newIndexNode;
				++iIter;
			}
			if (pKey < iIter->first)
				iIter->second = pIndexNode;
			--iIter;
		}
		newIndexNode->setParent(upIndexNode);
		pIndexNode->setParent(upIndexNode);
		newIndexNode->setMostLeftChild(pIndexNode->getMostLeftChild());
		// 여기 뭔가 하나 더 들어가야 하는데, 그냥 디버깅하면서 알아볼랭

		if (upIndexMap.size() > (order - 1))
			splitIndexNode(upIndexNode);	
	}
}

BpTreeNode* BpTree::searchDataNode(string name) {
	
	BpTreeNode* pCur = root;
	BpTreeNode* pNext;
	map <string, LoanBookData*>::iterator mIter;

	while(pCur->getMostLeftChild() != NULL) 
		pCur = pCur->getMostLeftChild();
	pNext = pCur;

	while(pCur != NULL) {

		if(pCur->getNext() == NULL)
			return pCur;
		
		if(name < pNext->getDataMap()->begin()->first)
			return pCur;

		pNext = pCur->getNext();
		pCur = pCur->getNext();
	}	
	return pCur;
}

bool BpTree::searchBook(string name) {

	BpTreeNode* pCur = root;
	while(pCur->getMostLeftChild() != NULL) 
		pCur = pCur->getMostLeftChild();
	
	map <string, LoanBookData*>::iterator mIter;
	while(pCur != NULL) {

		for(mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) 
			cout << mIter->first << " ";
		pCur = pCur->getNext();
		cout << endl << endl;
	}	
}

bool BpTree::searchRange(string start, string end) {
	
}
