#pragma once
#include "SelectionTree.h"
#include "BpTree.h"
#include <string>
#include <vector>

class Manager
{
private:
	char* cmd;
	BpTree* bptree;
	SelectionTree* stree;
	int bpOrder;
public:
	Manager(int bpOrder)	//constructor
	{
		this->bpOrder = bpOrder;
	}


	~Manager()//destructor
	{
		/* You must fill here */
	}

	ifstream fin;
	ofstream flog;
	

	void run(const char* command);
	bool LOAD();
	bool ADD();

	bool SEARCH_BP_BOOK(string book);
	bool SEARCH_BP_RANGE(string start, string end);

	bool PRINT_BP();
	bool PRINT_ST();

	bool DELETE();

	void printErrorCode(int n);
	void printSuccessCode();

};

