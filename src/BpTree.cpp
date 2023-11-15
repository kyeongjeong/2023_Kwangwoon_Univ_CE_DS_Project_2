#include "BpTree.h"
#include <cmath>

bool BpTree::Insert(LoanBookData* newData) {

 	if(root == NULL) { // if there is no data in B+ tree

		BpTreeDataNode* dataNode = new BpTreeDataNode(); // create new bptreenode
		dataNode->insertDataMap(newData->getName(), newData); // insert data in bptreenode
		root = dataNode; // set root
		return true;
	}

	BpTreeNode *pCur = root, *pPar;
	map <string, LoanBookData*>::iterator mIter;
	while(pCur->getMostLeftChild() != NULL) 
		pCur = pCur->getMostLeftChild();
	while(!pCur->isDataNode())
		pCur = pCur->getIndexMap()->begin()->second; // move to datanode
	pPar = pCur;
	
	while(pCur != NULL) { // traversal data node
		
		for(mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) { // traversal data map

			if(mIter->first == newData->getName()) {
					
					mIter->second->updateCount();

					// check loan-available
					// if not, delete data from B+ tree
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

	if(root->isDataNode()) { // if there is no index node

		root->insertDataMap(newData->getName(), newData); // insert data to root(data node)
		if(excessDataNode(root)) // check whether a data node needs to split
			splitDataNode(root);
		return true;
	}
	else { // it there are index node

		pCur = searchDataNode(newData->getName()); // find location to insert
		
		if(pCur == NULL) {
			BpTreeDataNode* dataNode = new BpTreeDataNode(); // create new bptreenode
			dataNode->insertDataMap(newData->getName(), newData); // insert data in bptreenode
			
			pPar = pPar->getParent();
			pPar->setMostLeftChild(dataNode); // set new node to mostleftchild of parent node
			dataNode->setParent(pPar);
			
			pPar->getIndexMap()->begin()->second->setPrev(dataNode); // connect next, prev node
			dataNode->setNext(pPar->getIndexMap()->begin()->second);
			pCur = dataNode;
		}
		else
			pCur->insertDataMap(newData->getName(), newData); // insert data to that data node
	
		if(excessDataNode(pCur)) // check whether a data node needs to split
			splitDataNode(pCur);
		
		pCur = pCur->getParent(); // move to parent index node
		while(pCur != NULL) {
			if(excessIndexNode(pCur)) // // check whether a index node needs to split
				splitIndexNode(pCur);
			pCur = pCur->getParent();
		}
		return true;
	}
	return false;
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
	if (pDataNode->getDataMap()->size() > order - 1) return true; // order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode->getIndexMap()->size() > order - 1) return true; // order is equal to the number of elements 
	else return false;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
	
	int splitKey = ceil((order-1)/2.0); // The location (key value) to split the data node
	BpTreeDataNode* newDataNode = new BpTreeDataNode();
	BpTreeIndexNode *newIndexNode;
	BpTreeNode *indexNode;
	map <string, LoanBookData*>::iterator mIter;
	map <string, LoanBookData*>::iterator mIter2;
	map<string, BpTreeNode *>::iterator iIter;
	
	mIter = pDataNode->getDataMap()->begin(); // traversal data map
	for(int i = 0; i < splitKey; i++) {

		newDataNode->insertDataMap(mIter->first, mIter->second); // create new data node
		mIter2 = mIter;
		mIter++;
		pDataNode->deleteMap(mIter2->first); // delete the iterator data in map
	}
	string pKey = mIter->first; // first key element of pDataNode

	if(pDataNode->getParent() == NULL) { // if there is no index node

		newIndexNode = new BpTreeIndexNode(); // create new index node
		root = newIndexNode; // set index node as root
		newIndexNode->insertIndexMap(pKey, pDataNode);	// insert data in index node map
		newIndexNode->setMostLeftChild(newDataNode); // set data node as mostleftchild of index node

		newDataNode->setParent(newIndexNode); // set parent, child
		pDataNode->setParent(newIndexNode);
	}
	else { // If there is a parent index node

        indexNode = pDataNode->getParent();
        indexNode->insertIndexMap(pKey, pDataNode); // Insert pDataNode into the parent index node with the key pKey

        iIter = indexNode->getIndexMap()->begin(); 
        if (newDataNode->getDataMap()->begin()->first < iIter->first)
            indexNode->setMostLeftChild(newDataNode); // Update the most left child if needed

        for (; iIter != indexNode->getIndexMap()->end(); iIter++) { 
     
            if (newDataNode->getDataMap()->begin()->first == iIter->first)
                iIter->second = newDataNode; // Update the corresponding child of the index node

            if (pKey == iIter->first)
                iIter->second = pDataNode; // Update the corresponding child of the index node
        }   
        newDataNode->setParent(indexNode); // Set parent and child relationships
        pDataNode->setParent(indexNode);
        if(pDataNode->getPrev() != NULL)
            pDataNode->getPrev()->setNext(newDataNode); // Update the next and previous pointers of adjacent data nodes
        newDataNode->setPrev(pDataNode->getPrev());

        if (indexNode->getIndexMap()->size() > (order - 1))
            splitIndexNode(indexNode); // Perform index node split if necessary
    }
    newDataNode->setNext(pDataNode); // Update the next and previous pointers of adjacent data nodes
    pDataNode->setPrev(newDataNode);
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {

    int splitKey = ceil((order - 1) / 2.0); // Calculate the split key, considering the order of the B+ tree
    BpTreeIndexNode* newIndexNode = new BpTreeIndexNode(); // Create a new index node to store the split data
    BpTreeNode *upIndexNode; // Pointer to a new parent index node
    map<string, BpTreeNode*>::iterator iIter; // Iterator for traversing the index map
    map<string, BpTreeNode*>::iterator iIter2; // Iterator for deleting elements from the index map

    iIter = pIndexNode->getIndexMap()->begin(); // Start iterating through the index map
    for(int i = 0; i < splitKey; i++) {

        newIndexNode->insertIndexMap(iIter->first, iIter->second); // Copy index data to the new index node
        if (pIndexNode->getMostLeftChild() != NULL) {
            newIndexNode->setMostLeftChild(pIndexNode->getMostLeftChild()); // Copy the most left child if it exists
            pIndexNode->getMostLeftChild()->setParent(newIndexNode); // Update the parent of the most left child
            pIndexNode->setMostLeftChild(NULL); // Set the most left child of the original index node to NULL
        }
        iIter->second->setParent(newIndexNode); // Update the parent of the copied child
        iIter2 = iIter;
        iIter++;
        pIndexNode->deleteMap(iIter2->first); // Remove the copied index data from the original index node
    }
    string pKey = iIter->first; // Retrieve the first key element of the remaining data in pIndexNode
    iIter2 = iIter++;
    pIndexNode->setMostLeftChild(iIter2->second); // Update the most left child of pIndexNode
    pIndexNode->deleteMap(iIter2->first); // Remove the copied index data from pIndexNode

    if(pIndexNode->getParent() == NULL) { // If there is no parent index node (root case)

        upIndexNode = new BpTreeIndexNode(); // Create a new parent index node
        root = upIndexNode; // Set the new index node as the root
        upIndexNode->insertIndexMap(pKey, pIndexNode); // Insert pIndexNode into the upIndexNode map with the key pKey
        upIndexNode->setMostLeftChild(newIndexNode); // Set newIndexNode as the most left child of the upIndexNode

        newIndexNode->setParent(upIndexNode); // Set parent and child relationships
        pIndexNode->setParent(upIndexNode);
    }
    else { // If there is a parent index node

        upIndexNode = pIndexNode->getParent();
        upIndexNode->insertIndexMap(pKey, pIndexNode); // Insert pIndexNode into the parent upIndexNode with the key pKey

        string nKey = newIndexNode->getIndexMap()->begin()->first; // Retrieve the first key of newIndexNode
        iIter = upIndexNode->getIndexMap()->begin();
        if (nKey < iIter->first)
            upIndexNode->setMostLeftChild(newIndexNode); // Update the most left child of upIndexNode if needed

        for(; iIter != upIndexNode->getIndexMap()->end(); iIter++) {

            if (iIter == upIndexNode->getIndexMap()->end()) {
                iIter->second = pIndexNode; // Update the corresponding child of upIndexNode
                break;
            }

            iIter2 = iIter++;
            if (nKey < iIter->first) {
                iIter2->second = newIndexNode; // Update the corresponding child of upIndexNode
                ++iIter;
                break;
            }
            --iIter;
        }

        newIndexNode->setParent(upIndexNode); // Set parent and child relationships
        pIndexNode->setParent(upIndexNode);
        newIndexNode->setMostLeftChild(pIndexNode->getMostLeftChild()); // Update the most left child of newIndexNode

        if (upIndexNode->getIndexMap()->size() > (order - 1))
            splitIndexNode(upIndexNode); // Perform index node split if necessary
    }
}

BpTreeNode* BpTree::searchDataNode(string name) {

    BpTreeNode* pCur = root; // Initialize the current node to the root
    BpTreeNode* pNext; // Pointer to the next node in the B+ tree
    map<string, LoanBookData*>::iterator mIter; // Iterator for traversing the data map

    while (pCur->getMostLeftChild() != NULL) 
        pCur = pCur->getMostLeftChild(); // Traverse to the leftmost child to find the starting point
    while (!pCur->isDataNode())
        pCur = pCur->getIndexMap()->begin()->second; // Traverse to the data node level
    pNext = pCur; // Set the next node to the current node

    if ((pCur != root) && (pCur != pCur->getParent()->getMostLeftChild()) && (name < pCur->getDataMap()->begin()->first))
        return NULL; // Return null if the search name is smaller than the first key in the leftmost data node

    while (pCur != NULL) {

        if (pCur->getNext() == NULL)
            return pCur; // Return the current node if there is no next node

        pNext = pCur->getNext(); // Move to the next node
        if (name < pNext->getDataMap()->begin()->first)
            return pCur; // Return the current node if the search name is smaller than the first key in the next node

        pCur = pCur->getNext(); // Move to the next node
    }   
    return pCur; // Return the current node
}

bool BpTree::searchBook(string name, bool isPrint) {

	BpTreeNode* pCur; // Pointer to the current node
    map<string, LoanBookData*>::iterator mIter; // Iterator for traversing the data map
    map<string, BpTreeNode*>::iterator iIter; // Iterator for traversing the index map
	//
	bool isBreak = false;

	pCur = searchDataNode(name); // Find the data node containing the search 'name'
    if (pCur == NULL)
        return false; // Return false if the search 'name' is not found in the B+ tree

	for(mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) {

		if(mIter->first == name) {

			if(isPrint == true) { // Print details if 'isPrint' is true
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
	// return false; // Return false if the search 'name' is not found in the current data node
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

    BpTreeNode* pCur; // Pointer to the current node
    map<string, LoanBookData*>::iterator mIter; // Iterator for traversing the data map
    bool isPrint = false; // Flag to indicate if printing is required
    string bookName, firstWord; // Variables to store book names and the first character of each book name
  
    pCur = searchDataNode(start); // Find the starting point in the B+ tree for the search range
    if (pCur == NULL)
        return false; // Return false if the starting point is not found in the B+ tree

    while (pCur != NULL) { // Iterate through the data nodes to find the books within the specified range

        for (mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) {

            bookName = mIter->first;
            firstWord = bookName.substr(0, 1);
            if ((firstWord.compare(start) >= 0) && (firstWord.compare(end) <= 0)) {  // Check if the book name is within the specified range
                isPrint = true; // Set the flag to indicate that printing is required
                break;
            }
        }
        if (isPrint == true)
            break;
        pCur = pCur->getNext(); // Move to the next data node
    }
    if (isPrint == false)
        return isPrint; // Return false if no books are found within the specified range

    // Print the details of the books within the specified range
    pCur = searchDataNode(start);
    *fout << "========SEARCH_BP========" << endl;
    while (pCur != NULL) {

        for (mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) {

            bookName = mIter->first;
            firstWord = bookName.substr(0, 1);
            if ((firstWord.compare(start) >= 0) && (firstWord.compare(end) <= 0)) { // Check if the book name is within the specified range and print the details
                *fout << mIter->second->getName() << "/" << mIter->second->getCode();
                if (mIter->second->getCode() == 0)
                    *fout << "00";
                *fout << "/" << mIter->second->getAuthor() << "/" << mIter->second->getYear() << "/" << mIter->second->getLoanCount() << endl;
            }
        }
        pCur = pCur->getNext(); // Move to the next data node
    }
    *fout << "=========================" << endl;
    return isPrint; // Return true if books are found within the specified range
}

bool BpTree::printBP() {

    BpTreeNode* pCur = root; // Pointer to the current node, starting from the root
    while (pCur->getMostLeftChild() != NULL)
        pCur = pCur->getMostLeftChild(); // Traverse to the leftmost child to find the starting point
    while (!pCur->isDataNode())
        pCur = pCur->getIndexMap()->begin()->second; // Traverse to the data node level

    map<string, LoanBookData*>::iterator mIter; // Iterator for traversing the data map
    while (pCur != NULL) {  // Iterate through the data nodes to print the details of all the books

        for (mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++) {
            *fout << mIter->second->getName() << "/" << mIter->second->getCode();
            if (mIter->second->getCode() == 0)
                *fout << "00";
            *fout << "/" << mIter->second->getAuthor() << "/" << mIter->second->getYear() << "/" << mIter->second->getLoanCount() << endl;
        }
        pCur = pCur->getNext(); // Move to the next data node
    }
    return true; // Return true to indicate successful printing
}

bool BpTree::Delete(BpTreeNode* dNode, string dName) { 

	bool isInIndex = false;
	BpTreeNode *pCur, *pIndex, *pSib, *pSwap;
	map <string, BpTreeNode*>::iterator iIter;
	map <string, LoanBookData*>::iterator mIter;

	pCur = dNode->getParent(); // find dName in index node
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

	// 1. dName does not exist in index node
	if(isInIndex == false) {

		if(dNode->getDataMap()->size() == 1) { // 1.1. dNode->size == 1(dNode is mostLeftChild in whole B+ tree)

			dNode->getParent()->setMostLeftChild(NULL);
			if(dNode->getNext() != NULL)
				dNode->getNext()->setPrev(NULL);
			delete dNode;
			return true;
		}
		dNode->deleteMap(dName); // 1.2. dNode->size > 1
		return true;
	}

	// 2. dName exists in index node
	if(dNode->getDataMap()->size() > 1) { // 2.1. underflow x

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
	
	// 2.2. underflow o
	// 2.2.1. reassignment o
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

	// 2.2.2. reassignment x(merge)
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