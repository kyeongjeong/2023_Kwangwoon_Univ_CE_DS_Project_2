#include "Manager.h"

void Manager::run(const char* command) 
{
	fin.open(command);
	flog.open("log.txt");
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
            if(!LOAD(bptree))
				printErrorCode(100);
        }
		else {
			printErrorCode(700);
		}
	}	
	fin.close();
	flog.close();
	return;
}

bool Manager::LOAD(BpTree* bptree)
{
	ifstream floan;
    floan.open("loan.txt");
	if(!floan) 
		return false;

	if(bptree->getRoot() == NULL)
		return false;
	
	string name, author;
	int code, year, loan_count;

    while (!floan.eof()) {

        floan >> name >> code >> author >> year >> loan_count;

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
		else
			return false;

		if(isLoanAvail == true)
			bptree->Insert(newData);
    }

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

void Manager::printSuccessCode() {//SUCCESS CODE PRINT 
	flog << "=======================" << endl;
	flog << "Success" << endl;
	flog << "=======================" << endl << endl;
}

