#include "BpTree.h"
#include <cmath>

bool BpTree::Insert(LoanBookData* newData) {

 	if(root == NULL) {

		BpTreeDataNode* dataNode = new BpTreeDataNode();
		dataNode->insertDataMap(newData->getName(), newData);
		root = dataNode;
		return true;
	}

	BpTreeNode* pCur = root;
	map <string, LoanBookData*>::iterator mIter;
	while(pCur->getMostLeftChild() != NULL) 
		pCur = pCur->getMostLeftChild();
	
	while(pCur != NULL) {

		for(mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) {

			if(mIter->first == newData->getName()) {
					
					mIter->second->updateCount();

					if((mIter->second->getCode() == 000) && (mIter->second->getLoanCount() == 3))
						;
					else if((mIter->second->getCode() == 100) && (mIter->second->getLoanCount() == 3))
						;
					else if((mIter->second->getCode() == 200) && (mIter->second->getLoanCount() == 3))
						;
					else if((mIter->second->getCode() == 300) && (mIter->second->getLoanCount() == 4))
						;
					else if((mIter->second->getCode() == 400) && (mIter->second->getLoanCount() == 4))
						;
					else if((mIter->second->getCode() == 500) && (mIter->second->getLoanCount() == 2))
						;
					else if((mIter->second->getCode() == 600) && (mIter->second->getLoanCount() == 2))
						;
					else if((mIter->second->getCode() == 700) && (mIter->second->getLoanCount() == 2))
						;
				return true;
			}
		}
		pCur = pCur->getNext();
	}

	if(root->getMostLeftChild() == NULL) {

		root->insertDataMap(newData->getName(), newData);
		if(excessDataNode(root))
			splitDataNode(root);
		return true;
	}

	else {

		pCur = searchDataNode(newData->getName());
		pCur->insertDataMap(newData->getName(), newData);
	
		if(excessDataNode(pCur))
			splitDataNode(pCur);
		
		pCur = pCur->getParent();
		while(pCur != NULL) {
			if(excessIndexNode(pCur))
				splitIndexNode(pCur);
			pCur = pCur->getParent();
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
		if(pDataNode->getPrev() != NULL)
			pDataNode->getPrev()->setNext(newDataNode);
		newDataNode->setPrev(pDataNode->getPrev());

		if (indexMap->size() > (order - 1))
		 	splitIndexNode(indexNode);
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
		iIter->second->setParent(newIndexNode);
		map <string, BpTreeNode*>::iterator iIter2 = iIter;
		iIter++;
		pIndexNode->deleteMap(iIter2->first);
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
		pIndexNode->getMostLeftChild()->setParent(newIndexNode);
		pIndexNode->setMostLeftChild(NULL);
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

			map <string, BpTreeNode*>::iterator iIter2 = iIter++;
			if (nKey < iIter->first) {
				iIter2->second = newIndexNode;
				break;
			}
			--iIter;
		}
		newIndexNode->setParent(upIndexNode);
		pIndexNode->setParent(upIndexNode);
		newIndexNode->setMostLeftChild(pIndexNode->getMostLeftChild());	

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
		
		pNext = pCur->getNext();
		if(name < pNext->getDataMap()->begin()->first)
			return pCur;

		pCur = pCur->getNext();
	}	
	return pCur;
}

bool BpTree::searchBook(string name, bool isPrint) {

	BpTreeNode* pCur = root;
	map <string, LoanBookData*>::iterator mIter;

	while(pCur->getMostLeftChild() != NULL) 
		pCur = pCur->getMostLeftChild();
	
	while(pCur != NULL) {

		for(mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) {

			if(mIter->first == name) {

				if(isPrint == true) {
					*fout << "========SEARCH_BP========" << endl;
					*fout << mIter->second->getName() << "/" << mIter->second->getCode();
					if(mIter->second->getCode() == 0)
						*fout << "00";
					*fout << "/" << mIter->second->getAuthor() << "/" << mIter->second->getYear() << "/" << mIter->second->getLoanCount() << endl;
					*fout << "=========================" << endl;
				}
				return true;
			}
		}
		pCur = pCur->getNext();
	}
	return false;
}

bool BpTree::searchRange(string start, string end) {
	
	BpTreeNode* pCur = root;
	map <string, LoanBookData*>::iterator mIter;
	bool isPrint = false;
	string bookName, firstWord;

	while(pCur->getMostLeftChild() != NULL) 
		pCur = pCur->getMostLeftChild();
	
	while(pCur != NULL) {

		for(mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) {

			bookName = mIter->first;
			firstWord = bookName.substr(0, 1);
			if((firstWord.compare(start) >= 0) && (firstWord.compare(end) <= 0)) {
				isPrint = true;
				break;
			}
		}
		if (isPrint == true)
			break;
		pCur = pCur->getNext();
	}
	if(isPrint == false)
		return isPrint;

	pCur = root;
	while(pCur->getMostLeftChild() != NULL) 
		pCur = pCur->getMostLeftChild();

	*fout << "========SEARCH_BP========" << endl;
	while(pCur != NULL) {

		for(mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) {

			bookName = mIter->first;
			firstWord = bookName.substr(0, 1);
			if((firstWord.compare(start) >= 0) && (firstWord.compare(end) <= 0)) {
				*fout << mIter->second->getName() << "/" << mIter->second->getCode();
				if(mIter->second->getCode() == 0)
					*fout << "00";
				*fout << "/" << mIter->second->getAuthor() << "/" << mIter->second->getYear() << "/" << mIter->second->getLoanCount() << endl;
			}
		}
		pCur = pCur->getNext();
	}
	*fout << "=========================" << endl;
	return isPrint;
}

bool BpTree::printBP() {
	
	BpTreeNode* pCur = root;
	while(pCur->getMostLeftChild() != NULL) 
		pCur = pCur->getMostLeftChild();
	
	map <string, LoanBookData*>::iterator mIter;
	while(pCur != NULL) {

		for(mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) {
			*fout << mIter->second->getName() << "/" << mIter->second->getCode();
			if(mIter->second->getCode() == 0)
				*fout << "00";
			*fout << "/" << mIter->second->getAuthor() << "/" << mIter->second->getYear() << "/" << mIter->second->getLoanCount() << endl;
		}
		pCur = pCur->getNext();
	}
	return true;	
}