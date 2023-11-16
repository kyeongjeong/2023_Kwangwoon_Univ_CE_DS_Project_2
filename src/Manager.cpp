#include "Manager.h"

void Manager::run(const char* command) 
{
	fin.open(command); // open command file
	flog.open("log.txt", ios::app); // open log file
	if(!fin) // if fin was not found
	{
		flog << "File Open Error" << endl;
		return;
	}

	bptree = new BpTree(&flog, bpOrder); // create B+ Tree
	stree = new SelectionTree(&flog); // create selection tree

	while (!fin.eof()) // read fin file
	{
		string cmd, line;
		getline(fin, line);
		if (!line.empty() && line.back() == '\r') // if you encounter '/r', ignore it
        	line.pop_back();

		vector<string> tokens;
		char* token = strtok(&line[0], "\t"); 

		while (token != NULL) { // read in units of '\t' separation
			tokens.push_back(token);
			token = strtok(NULL, "\t");
		}
		cmd = tokens[0];

        if (cmd == "LOAD") { // if command is LOAD
            if((tokens.size() != 1) || (!LOAD())) // fail to LOAD or too many parameter
				printErrorCode(100);
        }

		else if (cmd == "ADD") { // if command is ADD

			string name, author;
			int code, year;

			if (tokens.size() == 5) { // read command
				name = tokens[1];
				code = stoi(tokens[2]);
				author = tokens[3];
				year = stoi(tokens[4]);

				if(!ADD(name, code, author, year)) // fail to ADD
					printErrorCode(200);
			}
			else // if there are too less or too many parameters...
				printErrorCode(200);
        }

		else if (cmd == "SEARCH_BP") { // if command is SEARCH_BP
 
			string name, start, end;

			if (tokens.size() == 2) { // if number of parameter is one
				name = tokens[1];
				if(!SEARCH_BP(name, "", "")) // invoke SEARCH_BP as bookname
					printErrorCode(300);
			}
			else if (tokens.size() == 3) { // if number of parameter is two
				start = tokens[1];
				end = tokens[2];
				if(!SEARCH_BP("", start, end)) // invoke SEARCH_BP as range
					printErrorCode(300);
			}
			else // if there are too less or too many parameters...
				printErrorCode(300);
		}

		else if (cmd == "PRINT_BP") { // if command is PRINT_BP
			if((tokens.size() != 1) || (!PRINT_BP())) // fail to PRINT_BP
				printErrorCode(400);
		}

		else if(cmd == "PRINT_ST") { // if command is PRINT_ST
			
			int code;
			if (tokens.size() == 2) {
				code = stoi(tokens[1]);
				if(!PRINT_ST(code)) // fail to PRINT_ST
					printErrorCode(500);
			}
			else // if there are too less or too many parameters...
				printErrorCode(500);
		}

		else if(cmd == "DELETE") { // if command is DELETE
			if((tokens.size() != 1) || (!DELETE())) // fail to DELETE
				printErrorCode(600);
		}

		else if(cmd == "EXIT") { // if command is EXIT
			printSuccessCode("EXIT");
			fin.close(); // close files
			flog.close();
			return; // Terminate the program
		}

		else 
			printErrorCode(700); // if other command...
	}	
	fin.close(); // close files
	flog.close();
	return;  // Terminate the program
}

bool Manager::LOAD()
{
	ifstream floan;
    floan.open("loan_book2.txt"); // open loan_book file
	if(!floan) // file open fail 
		return false;

	if((bptree->getRoot() != NULL) || (stree->getRoot() != NULL)) // if data already exits
		return false;
	
	string name, author;
	int code, year, loan_count;

    string line;
	while (getline(floan, line)) { // read the line
 
		if (!line.empty() && line.back() == '\r') // if you encounter '/r', ignore it
            line.pop_back();

		vector<string> tokens;
		char* token = strtok(&line[0], "\t");

        while (token != NULL) { // read all data
            tokens.push_back(token);
            token = strtok(NULL, "\t");
        }

        if (tokens.size() == 5) { // store data if number of tokens are 5
            name = tokens[0];
            code = stoi(tokens[1]);
            author = tokens[2];
            year = stoi(tokens[3]);
            loan_count = stoi(tokens[4]);
		}
		else // if there are too less or too many parameters...
			return false;

		LoanBookData *newData = new LoanBookData(); // create new LoanBookData and set data
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

		if(isLoanAvail == true) { // if book was loan-avilable
			if (!bptree->Insert(newData)) // insert to B+ tree
				return false;
		}
		else { // if not
			if (!stree->Insert(newData)) // insert to selection tree
				return false;
		}
    }
	printSuccessCode("LOAD");
	floan.close(); // file close
	return true; // terminate fuction
}

bool Manager::ADD(string name, int code, string author, int year)
{
	LoanBookData *newData = new LoanBookData(); // create new LoanBookData
	newData->setBookData(name, code, author, year, 0); // set data in LoanBookData
	if(!bptree->Insert(newData)) // if book was not loan-available
		stree->Insert(newData); // insert to selection tree

	flog << "=========ADD=========" << endl; // print the added data
	flog << newData->getName() << "/" << newData->getCode();
	if(newData->getCode() == 0)
		flog << "00";
	flog << "/" << newData->getAuthor() << "/" << newData->getYear() << endl;
	flog << "======================" << endl << endl;
	return true; // terminate function
}

bool Manager::SEARCH_BP(string name, string start, string end) 
{
	if(bptree->getRoot() == NULL)
		return false;

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
	if(bptree->getRoot() == NULL) // if B+ tree was empty
		return false;
	flog << "========PRINT_BP========" << endl;
	bptree->printBP(); // print all data of B+ tree
	flog << "========================" << endl << endl;
	return true;
}

bool Manager::PRINT_ST(int code) 
{
	// if code was wrong
	if ((code != 0) && (code != 100) && (code != 200) && (code != 300) && (code != 400) && (code != 500) && (code != 600) && (code != 700))
		return false;
	
	// if there is no data in selection tree
	if((stree->getRoot() == NULL) || (stree->getRoot()->getBookData() == NULL))
		return false;
	
	// fail to find and print book data
	if(!stree->printBookData(code))
		return false;

	return true;
}

bool Manager::DELETE() 
{
	// if there is no data in selection tree
	if((stree->getRoot() == NULL) || (stree->getRoot()->getBookData() == NULL))
		return false;
	
	// fail to delete data in selection tree
	if(!stree->Delete())
		return false;

	printSuccessCode("DELETE");
	return true;
}

void Manager::printErrorCode(int n) { //ERROR CODE PRINT
	flog << "=========ERROR=========" << endl;
	flog << n << endl;
	flog << "=======================" << endl << endl;
}

void Manager::printSuccessCode(string command) { //SUCCESS CODE PRINT 
	flog << "=========" << command << "=========" << endl;
	flog << "Success" << endl;
	flog << "=======================" << endl << endl;
}