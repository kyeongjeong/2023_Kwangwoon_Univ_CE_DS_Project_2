#include "Manager.h"

void Manager::run(const char* command) 
{
	fin.open(command);
	flog.open("log.txt", ios::app);
	if(!fin)
	{
		flog << "File Open Error" << endl;
		return;
	}

	bptree = new BpTree(&flog, bpOrder);

	while (!fin.eof())
	{
		string cmd;
        fin >> cmd; // Read the command

        if (cmd == "LOAD") {
            if(!LOAD())
				printErrorCode(100);
        }
		else if (cmd == "PRINT_BP") {
			if(!PRINT_BP())
				printErrorCode(400);
		}
		else {
			printErrorCode(700);
		}
	}	
	fin.close();
	flog.close();
	return;
}

bool Manager::LOAD()
{
	ifstream floan;
    floan.open("loan.txt");
	if(!floan) 
		return false;

	if(bptree->getRoot() != NULL)
		return false;
	
	string name, author;
	int code, year, loan_count;

    string line;
	while (getline(floan, line)) {

		if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

		vector<string> tokens;
		char* token = strtok(&line[0], "\t");

        while (token != NULL) {
            tokens.push_back(token);
            token = strtok(NULL, "\t");
        }

        if (tokens.size() == 5) {
            name = tokens[0];
            code = stoi(tokens[1]);
            author = tokens[2];
            year = stoi(tokens[3]);
            loan_count = stoi(tokens[4]);
		}
		else
			return false;

		LoanBookData *newData = new LoanBookData();
		newData->setBookData(name, code, author, year);

		bool isLoanAvail = true;
		if((code == 000) && (loan_count == 3))
			isLoanAvail = false;
		else if((code == 100) && (loan_count == 3))
			isLoanAvail = false;
		else if((code == 200) && (loan_count == 3))
			isLoanAvail = false;
		else if((code == 300) && (loan_count == 4))
			isLoanAvail = false;
		else if((code == 400) && (loan_count == 4))
			isLoanAvail = false;
		else if((code == 500) && (loan_count == 2))
			isLoanAvail = false;
		else if((code == 600) && (loan_count == 2))
			isLoanAvail = false;
		else if((code == 700) && (loan_count == 2))
			isLoanAvail = false;

		if(isLoanAvail == true)
			bptree->Insert(newData);
    }
	printSuccessCode("LOAD");
	return true;
}

bool Manager::ADD()
{
	
	return true;
}

bool Manager::SEARCH_BP_BOOK(string book) 
{
	
}

bool Manager::SEARCH_BP_RANGE(string s, string e) 
{
	
}

bool Manager::PRINT_BP() 
{
	if(bptree->getRoot() == NULL)
		return false;
	flog << "========PRINT_BP========" << endl;
	bptree->printBP();
	flog << "========================" << endl << endl;
	return true;
}

bool Manager::PRINT_ST() 
{

}

bool Manager::DELETE() 
{

}

void Manager::printErrorCode(int n) {				//ERROR CODE PRINT
	flog << "=========ERROR=========" << endl;
	flog << n << endl;
	flog << "=======================" << endl << endl;
}

void Manager::printSuccessCode(string command) {//SUCCESS CODE PRINT 
	flog << "=========" << command << "=========" << endl;
	flog << "Success" << endl;
	flog << "=======================" << endl << endl;
}

