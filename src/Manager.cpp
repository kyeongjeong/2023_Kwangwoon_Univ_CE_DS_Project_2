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
	stree = new SelectionTree(&flog);

	while (!fin.eof())
	{
		string cmd, line;
		getline(fin, line);
		if (!line.empty() && line.back() == '\r') 
        	line.pop_back();

		vector<string> tokens;
		char* token = strtok(&line[0], "\t");

		while (token != NULL) {
			tokens.push_back(token);
			token = strtok(NULL, "\t");
		}
		cmd = tokens[0];

        if (cmd == "LOAD") {
            if((tokens.size() != 1) || (!LOAD()))
				printErrorCode(100);
        }
		else if (cmd == "ADD") {

			string name, author;
			int code, year;

			if (tokens.size() == 5) {
				name = tokens[1];
				code = stoi(tokens[2]);
				author = tokens[3];
				year = stoi(tokens[4]);

				if(!ADD(name, code, author, year))
					printErrorCode(200);
			}
			else
				printErrorCode(200);
        }
		else if (cmd == "SEARCH_BP") {

			string name, start, end;

			if (tokens.size() == 2) {         
				name = tokens[1];
				if(!SEARCH_BP(name, "", ""))
					printErrorCode(300);
			}
			else if (tokens.size() == 3) {
				start = tokens[1];
				end = tokens[2];
				if(!SEARCH_BP("", start, end))
					printErrorCode(300);
			}
			else
				printErrorCode(300);
		}
		else if (cmd == "PRINT_BP") {
			if((tokens.size() != 1) || (!PRINT_BP()))
				printErrorCode(400);
		}
		else if(cmd == "PRINT_ST") {
			if((tokens.size() != 1) || (!PRINT_ST()))
				printErrorCode(500);
		}
		else if(cmd == "DELETE") {
			if((tokens.size() != 1) || (!DELETE()))
				printErrorCode(600);
		}
		else if(cmd == "EXIT") {
			printSuccessCode("EXIT");
			fin.close();
			flog.close();
			return;
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
    floan.open("loan_book.txt");
	if(!floan) 
		return false;

	if((bptree->getRoot() != NULL) || (stree->getRoot() != NULL))
		return false;
	
	string name, author;
	int code, year, loan_count;

    string line;
	while (getline(floan, line)) {
 
		if (!line.empty() && line.back() == '\r') 
            line.pop_back();

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
		newData->setBookData(name, code, author, year, loan_count);

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

		if(isLoanAvail == true) {
			if (!bptree->Insert(newData))
				return false;
		}
		else {
			if (!stree->Insert(newData))
				return false;
		}
    }
	printSuccessCode("LOAD");
	floan.close();
	return true;
}

bool Manager::ADD(string name, int code, string author, int year)
{
	LoanBookData *newData = new LoanBookData();
	newData->setBookData(name, code, author, year, 0);
	if(!bptree->Insert(newData))
		stree->Insert(newData);

	flog << "=========ADD=========" << endl;
	flog << newData->getName() << "/" << newData->getCode();
	if(newData->getCode() == 0)
		flog << "00";
	flog << "/" << newData->getAuthor() << "/" << newData->getYear() << endl;
	flog << "======================" << endl << endl;
	return true;
}

bool Manager::SEARCH_BP(string name, string start, string end) 
{
	
    if (!name.empty() && start.empty() && end.empty()) {         
		if (!bptree->searchBook(name, true))
			return false;
	}
	else if (name.empty() && !start.empty() && !end.empty()) {
		if(!bptree->searchRange(start, end))
			return false;
	}
	else
		return false;
	return true;
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
	string line;
	getline(fin, line);
	if (!line.empty() && line.back() == '\r') 
        line.pop_back();

	vector<string> tokens;
	char* token = strtok(&line[0], "\t");

    while (token != NULL) {
        tokens.push_back(token);
        token = strtok(NULL, "\t");
    }

	int code;
    if (tokens.size() == 1) 
        code = stoi(tokens[0]);
	else
		return false;

	if ((code != 0) && (code != 100) && (code != 200) && (code != 300) && (code != 400) && (code != 500) && (code != 600) && (code != 700))
		return false;
	
	if(stree->getRoot() == NULL)
		return false;
	
	if(!stree->printBookData(code))
		return false;

	return true;
}

bool Manager::DELETE() 
{
	if(stree->getRoot() == NULL)
		return false;
	
	if(!stree->Delete())
		return false;

	printSuccessCode("DELETE");
	return true;
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