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
	while(!pCur->isDataNode())
		pCur = pCur->getIndexMap()->begin()->second;
	
	while(pCur != NULL) {

		for(mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) {

			if(mIter->first == newData->getName()) {
					
					mIter->second->updateCount();

					if((mIter->second->getCode() == 000) && (mIter->second->getLoanCount() == 3)) {
						Delete(pCur, mIter->first);
						return false;
					}
					else if((mIter->second->getCode() == 100) && (mIter->second->getLoanCount() == 3)) {
						Delete(pCur, mIter->first);
						return false;
					}
					else if((mIter->second->getCode() == 200) && (mIter->second->getLoanCount() == 3)) {
						Delete(pCur, mIter->first);
						return false;
					}
					else if((mIter->second->getCode() == 300) && (mIter->second->getLoanCount() == 4)) {
						Delete(pCur, mIter->first);
						return false;
					}
					else if((mIter->second->getCode() == 400) && (mIter->second->getLoanCount() == 4)) {
						Delete(pCur, mIter->first);
						return false;
					}
					else if((mIter->second->getCode() == 500) && (mIter->second->getLoanCount() == 2)) {
						Delete(pCur, mIter->first);
						return false;
					}
					else if((mIter->second->getCode() == 600) && (mIter->second->getLoanCount() == 2)) {
						Delete(pCur, mIter->first);
						return false;
					}
					else if((mIter->second->getCode() == 700) && (mIter->second->getLoanCount() == 2)) {
						Delete(pCur, mIter->first);
						return false;
					}
				return true;
			}
		}
		pCur = pCur->getNext();
	}

	if(root->isDataNode()) {

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
		return true;
	}
	return false;
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
	BpTreeIndexNode *newIndexNode;
	BpTreeNode *indexNode;
	map <string, LoanBookData*>::iterator mIter;
	map <string, LoanBookData*>::iterator mIter2;
	map<string, BpTreeNode *>::iterator iIter;
	
	mIter = pDataNode->getDataMap()->begin();
	for(int i = 0; i < splitKey; i++) {

		newDataNode->insertDataMap(mIter->first, mIter->second);
		mIter2 = mIter;
		mIter++;
		pDataNode->deleteMap(mIter2->first);
	}
	string pKey = mIter->first;

	if(pDataNode->getParent() == NULL) {

		newIndexNode = new BpTreeIndexNode();
		root = newIndexNode;
		newIndexNode->insertIndexMap(pKey, pDataNode);	
		newIndexNode->setMostLeftChild(newDataNode);

		newDataNode->setParent(newIndexNode);
		pDataNode->setParent(newIndexNode);
	}
	else {

		indexNode = pDataNode->getParent();
		indexNode->insertIndexMap(pKey, pDataNode);
		
		iIter = indexNode->getIndexMap()->begin();
		if (newDataNode->getDataMap()->begin()->first < iIter->first)
			indexNode->setMostLeftChild(newDataNode);

		for (; iIter != indexNode->getIndexMap()->end(); iIter++) { 
	 	
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

		if (indexNode->getIndexMap()->size() > (order - 1))
		 	splitIndexNode(indexNode);
	}
	newDataNode->setNext(pDataNode);
	pDataNode->setPrev(newDataNode);
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	
	int splitKey = ceil((order-1)/2.0);
	int count = 0;
	BpTreeIndexNode* newIndexNode = new BpTreeIndexNode();
	BpTreeNode *upIndexNode;
	map <string, BpTreeNode*>::iterator iIter;
	map <string, BpTreeNode*>::iterator iIter2;
	
	iIter = pIndexNode->getIndexMap()->begin();
	for(int i = 0; i < splitKey; i++) {

		newIndexNode->insertIndexMap(iIter->first, iIter->second);
		if(pIndexNode->getMostLeftChild() != NULL) {
			newIndexNode->setMostLeftChild(pIndexNode->getMostLeftChild());
			pIndexNode->getMostLeftChild()->setParent(newIndexNode);
			pIndexNode->setMostLeftChild(NULL);
		}
		iIter->second->setParent(newIndexNode);
		iIter2 = iIter;
		iIter++;
		pIndexNode->deleteMap(iIter2->first);
	}
	string pKey = iIter->first;
	iIter2 = iIter++;
	pIndexNode->setMostLeftChild(iIter2->second);
	pIndexNode->deleteMap(iIter2->first);
	

	if(pIndexNode->getParent() == NULL) {
		
		upIndexNode = new BpTreeIndexNode();
		root = upIndexNode;
		upIndexNode->insertIndexMap(pKey, pIndexNode);	
		upIndexNode->setMostLeftChild(newIndexNode);

		newIndexNode->setParent(upIndexNode);
		pIndexNode->setParent(upIndexNode);

	}
	else {

		upIndexNode = pIndexNode->getParent();
		upIndexNode->insertIndexMap(pKey, pIndexNode);
		string nKey = newIndexNode->getIndexMap()->begin()->first;

		iIter = upIndexNode->getIndexMap()->begin();
		if(nKey < iIter->first)
			upIndexNode->setMostLeftChild(newIndexNode);

		for(; iIter != upIndexNode->getIndexMap()->end(); iIter++) {

			if (iIter == upIndexNode->getIndexMap()->end()) {
				iIter->second = pIndexNode;
				break;
			}

			iIter2 = iIter++;
			if (nKey < iIter->first) {
				iIter2->second = newIndexNode;
				++iIter;
				break;
			}
			--iIter;
		}

		newIndexNode->setParent(upIndexNode);
		pIndexNode->setParent(upIndexNode);
		newIndexNode->setMostLeftChild(pIndexNode->getMostLeftChild());	

		if (upIndexNode->getIndexMap()->size() > (order - 1))
			splitIndexNode(upIndexNode);	
	}
}

BpTreeNode* BpTree::searchDataNode(string name) {
	
	BpTreeNode* pCur = root;
	BpTreeNode* pNext;
	map <string, LoanBookData*>::iterator mIter;

	while(pCur->getMostLeftChild() != NULL) 
		pCur = pCur->getMostLeftChild();
	while(!pCur->isDataNode())
		pCur = pCur->getIndexMap()->begin()->second;
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

	BpTreeNode* pCur;
	map <string, LoanBookData*>::iterator mIter;
	map <string, BpTreeNode*>::iterator iIter;
	//
	bool isBreak = false;

	pCur = searchDataNode(name);
	for(mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) {

		if(mIter->first == name) {

			if(isPrint == true) {
				*fout << "========SEARCH_BP========" << endl;
				*fout << mIter->second->getName() << "/" << mIter->second->getCode();
				if(mIter->second->getCode() == 0)
					*fout << "00";
				*fout << "/" << mIter->second->getAuthor() << "/" << mIter->second->getYear() << "/" << mIter->second->getLoanCount() << endl;
				*fout << "=========================" << endl;
				// return true;
			}
			//
			isBreak = true;
		}
	}
	// return false;
	//
	if(isBreak == false)
		return isBreak;

	*fout << "이동 경로(" << pCur->getDataMap()->begin()->first << ")";
	while(pCur->getParent() != NULL) {

		pCur = pCur->getParent();
		*fout << " -> ";
		int size = pCur->getIndexMap()->size();
		for(iIter = pCur->getIndexMap()->begin(); iIter != pCur->getIndexMap()->end(); iIter++)
			*fout << iIter->first << " ";
	}
	*fout << endl << endl;
	return isBreak;
}

bool BpTree::searchRange(string start, string end) {
	
	BpTreeNode* pCur;
	map <string, LoanBookData*>::iterator mIter;
	bool isPrint = false;
	string bookName, firstWord;
	
	pCur = searchDataNode(start);
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

	pCur = searchDataNode(start);
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
	while(!pCur->isDataNode())
		pCur = pCur->getIndexMap()->begin()->second;
	
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

bool BpTree::Delete(BpTreeNode* dNode, string dName) {

	bool isInIndex = false;
	BpTreeNode *pCur, *pIndex, *pSib, *pSwap;
	map <string, BpTreeNode*>::iterator iIter;
	map <string, LoanBookData*>::iterator mIter;

	pCur = dNode->getParent();
	while(pCur != NULL) {
		for(iIter = pCur->getIndexMap()->begin(); iIter != pCur->getIndexMap()->end(); iIter++) {
			if(iIter->first == dName) {
				pIndex = pCur;
				isInIndex = true;
				break;
			}
		}
		pCur = pCur->getParent();
	}
	pCur = dNode;

	// 1. index에 존재 x
	if(isInIndex == false) {

		if(dNode->getDataMap()->size() == 1) { // 1.1. dNode->size == 1(dNode가 가장 왼쪽의 mostLeftChild)

			dNode->getParent()->setMostLeftChild(NULL);
			if(dNode->getNext() != NULL)
				dNode->getNext()->setPrev(NULL);
			delete dNode;
			return true;
		}
		dNode->deleteMap(dName); // 1.2. dNode->size > 1
		return true;
	}

	// 2. index에 존재
	if(dNode->getDataMap()->size() > 1) { // 2.1. underflow가 발생하지 x

		mIter = dNode->getDataMap()->begin();
		if(mIter->first == dName)
			mIter++;
		string swapName = mIter->first;

		pCur = pIndex;
		pCur->insertIndexMap(swapName, dNode);
		pCur->deleteMap(dName);
		dNode->deleteMap(dName);
		return true;
	}
	
	// 2.2. underflow 발생
	// 2.2.1. 재배치 o
	if(((dNode->getNext() != NULL) && (dNode->getNext()->getParent() == dNode->getParent()) && (dNode->getNext()->getDataMap()->size() > 1)) || ((dNode->getPrev() != NULL) && (dNode->getPrev()->getParent() == dNode->getParent()) && (dNode->getPrev()->getDataMap()->size() > 1))) {
		
		string swapName, upName;
		if((dNode->getNext() != NULL) && (dNode->getNext()->getParent() == dNode->getParent()) && (dNode->getNext()->getDataMap()->size() > 1)) {

			pSib = pCur->getNext(); 
			mIter = pSib->getDataMap()->begin(); 
			swapName = mIter->first;
			upName = (++mIter)->first;
 
			pCur->insertDataMap(swapName, pSib->getDataMap()->begin()->second); 
		}
		else if((dNode->getPrev() != NULL) && (dNode->getPrev()->getParent() == dNode->getParent()) && (dNode->getPrev()->getDataMap()->size() > 1)) {

			pSib = pCur->getPrev();
			mIter = pSib->getDataMap()->end(); 
			swapName = (--mIter)->first;
			pCur->insertDataMap(swapName, mIter->second);

			upName = (--mIter)->first;
		}	
		pSib->deleteMap(swapName);

		bool isBreak = false;
		pCur = pCur->getParent();
		while(pCur != NULL) {
			
			for(iIter = pCur->getIndexMap()->begin(); iIter != pCur->getIndexMap()->end(); iIter++) {
				if(iIter->first == swapName) {
					isBreak = true;
					break;
				}
			}
			if(isBreak == true)
				break;
			pCur = pCur->getParent();
		}

		if(pCur != NULL) {
			pCur->insertIndexMap(upName, iIter->second);
			pCur->deleteMap(swapName);
		}

		for(iIter = pIndex->getIndexMap()->begin(); iIter != pIndex->getIndexMap()->end(); iIter++) {

			if(iIter->first == dName) {
				pIndex->insertIndexMap(swapName, iIter->second);
				pIndex->deleteMap(dName);
				dNode->deleteMap(dName);
				return true;
			}
		}
		return false;
	}

	// 2.2.2. 재배치 x(병합)
	if((dNode->getNext() != NULL) && (dNode->getNext()->getParent() == dNode->getParent()) && (dNode->getNext()->getDataMap()->size() == 1)) {
		
		pSib = pCur->getNext();
		string swapName = pSib->getDataMap()->begin()->first;

		pCur->insertDataMap(swapName, pSib->getDataMap()->begin()->second);
		pCur->setNext(pSib->getNext());
		if(pSib->getNext() != NULL)
			pSib->setPrev(pCur);
		delete pSib;

		pIndex->getMostLeftChild()->insertIndexMap(dName, pCur);
		pCur->setParent(pIndex->getMostLeftChild());
		if(pIndex == root) {
			root = pIndex->getMostLeftChild();
			pIndex->getMostLeftChild()->setParent(NULL);
		}
		if(pIndex->getParent() != NULL) {
			for(iIter = pIndex->getParent()->getIndexMap()->begin(); iIter != pIndex->getParent()->getIndexMap()->end(); iIter++) {
				if(iIter->first == dName)
					iIter->second = pIndex->getMostLeftChild();
			}
			if(pIndex->getParent()->getMostLeftChild() == pIndex)
				pIndex->getParent()->setMostLeftChild(pIndex->getMostLeftChild());
			pIndex->getMostLeftChild()->setParent(pIndex->getParent());
		}
		pCur = pIndex;
		pIndex = pIndex->getMostLeftChild();	
		delete pCur;
		
		for(iIter = pIndex->getIndexMap()->begin(); iIter != pIndex->getIndexMap()->end(); iIter++) {
			if(iIter->first == dName) {
				pIndex->insertIndexMap(swapName, iIter->second);
				iIter->second->deleteMap(dName);
				break;
			}
		}
		pIndex->deleteMap(dName);
		return true;
	}
	return false;
}