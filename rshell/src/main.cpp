#include <iterator>
#include <stack>
#include <queue>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#include "Pipe.cpp"
#include "InputRed.cpp"
#include "DoubleOutRed.cpp"
#include "SingleOutRed.cpp"
#include "Test.cpp"
#include "Exit.cpp"
#include "Semicolon.cpp"
#include "OR.cpp"
#include "AND.cpp"
#include "Connector.cpp"
#include "Cmd.cpp"
#include "Base.h"

using namespace std;

const string AND_STRING = "&&";
const string OR_STRING = "||";
const string SEMI_STRING = ";";
const string INPUT_RED = "<";
const string SINGLE_OUTPUT_RED = ">";
const string DOUBLE_OUTPUT_RED = ">>";
const string PIPE = "|";

void splitUpFirstCharacter(char* p) {
    string tempP = string(p);
    tempP = tempP.substr(1, tempP.size() - 1);
    strcpy(p, tempP.c_str());
}

void splitUpLastCharacter(char* p) {
    string tempP = string(p);
    size_t comments = tempP.find('#');
    size_t comma = tempP.find(';');
    if(comma != std::string::npos || comments != std::string::npos) {
        tempP.erase(tempP.end() - 2);
    }
    else {
        tempP.erase(tempP.end() - 1);
    }
    strcpy(p, tempP.c_str());
}

void splitUpLastCharacterAlways(char *p) {
    string tempP = string(p);
    tempP.erase(tempP.end() - 1);
    strcpy(p, tempP.c_str());
}

bool checkCon(char *q) {
    vector<string> s;
    s.push_back("&&");
    s.push_back("||");
    s.push_back("|");
    s.push_back(">");
    s.push_back(">>");
    s.push_back("<");

    for(unsigned i = 0; i < s.size(); i++) {
        if(q == s.at(i)) {
            return false;
        }
    }
    return true;
}

string SepCommands(char *q) {
    string Q = string(q);
    string results = "";

    for(string::iterator it = Q.begin(); it < Q.end(); it++) {
        
        if(*it == '<') {
            results += " < ";
        }
        
        else if(*it == '>') {
            string::iterator temp = it;
            temp++;
            if (*temp == '>') {
                results += " >> ";
                it = temp;
            }
            else {
                results += " > ";
            }
        }
        
        else if (*it == '|') {
            string::iterator temp = it;
            temp++;
            if (*temp == '|') {
                results += " || ";
                it = temp;
            }
            else {
                results += " | ";
            }
        }
        
        else if (*it == ')') {

        }
        else {
            results += *it;
        }
    }
    return results;
}

bool checkAllCon(char *q) {
    vector<string> s;
    s.push_back("&&");
    s.push_back("||");
    s.push_back("#");
    s.push_back(";");
    s.push_back("|");
    s.push_back(">");
    s.push_back(">>");
    s.push_back("<");

    for(unsigned i = 0; i < s.size(); i++) {
        if (q == s.at(i)) {
            return false;
        }
    }
    return true;
}

Base* grabTree(char *cstr) {
    string StringCstr = string(cstr);
    size_t findInput = StringCstr.find('<');
    size_t findOutput = StringCstr.find('>');
    size_t findPipe = StringCstr.find('|');
    if (findInput != std::string::npos || findOutput != std::string::npos || findPipe != std::string::npos) {
        StringCstr = SepCommands(cstr);
        strcpy(cstr, StringCstr.c_str());
    }

    queue<Base *> commandList;
    queue<char *> connectorList;
    queue<Connector *> completedListToRun;
	char *p = strtok(cstr, " ");

    bool firstArgSemi = false;
    char *checkingSemi = (char *) memchr(p, ';', strlen(p));
    char *checkingComment = (char *) memchr(p, '#', strlen(p));
    char *checkingTest = (char *) memchr(p, '[', strlen(p));
    string checkingStringTest = string(p);

    if (checkingSemi != NULL) {

        string tempP = string(p);
        tempP = tempP.substr(0, tempP.size() - 1);
        strcpy(p, tempP.c_str());

        string semiCol = ";";
        char *pushConnector = new char[2];
        strcpy(pushConnector, semiCol.c_str());
        connectorList.push(pushConnector);

        firstArgSemi = true;
    }

    if (checkingStringTest.compare("test") == 0) {
        p = strtok(NULL, " ");
        Test *c = new Test();

        char *checkFlag = (char *) memchr(p, '-', strlen(p));
        if (checkFlag != NULL) {
            c -> add_flag(p);
            p = strtok(NULL, " ");
            c -> add_flag(p);
            p = strtok(NULL, " ");
        }
        else {
            c -> add_flag(p);
            p = strtok(NULL, " ");
        }

        commandList.push(c);

        if (p != 0) {
            bool checkingConnectors = checkAllCon(p);
            char *commentP = (char *) memchr(p, '#', strlen(p));

            if (commentP != NULL) {
                checkingComment = commentP;
            }
            else if (checkingConnectors) {
                cout << "Error: Expected a connector, Received: " << p << endl;
                exit(1);
            }
            else {
                connectorList.push(p);
                p = strtok(NULL, " ");
            }
        }
    }

    if (checkingTest != NULL) {
        Test *c = new Test();

        p = strtok(NULL, " ");

        char *checkFlag = (char *) memchr(p, '-', strlen(p));
        if (checkFlag != NULL) {
            c -> add_flag(p);
            p = strtok(NULL, " ");
            c -> add_flag(p);
            p = strtok(NULL, " ");
        }
        else {
            c -> add_flag(p);
            p = strtok(NULL, " ");
        }
        if (p != 0) {
            char *checkingEndTest = (char *) memchr(p, ']', strlen(p));
            if (checkingEndTest == NULL) {
                cout << "Error: Expected: ']', Received: " << p << endl;
                exit(1);
            }
        }
        
        p = strtok(NULL, " ");
        commandList.push(c);

        if (p != 0) {
            bool checkingConnectors = checkAllCon(p);
            if (checkingConnectors) {
                cout << "Error: Expected a connector, Received: " << p << endl;
                exit(1);
            }
            char *commentP = (char *) memchr(p, '#', strlen(p));
            if (commentP != NULL) {
                checkingComment = commentP;
            }
            else {
                connectorList.push(p);
                p = strtok(NULL, " ");
            }
        }
    }


    if (checkingComment == NULL) {
        bool commentDetected = false;
        while (p != 0 && !commentDetected) {
            char *commentP = (char *) memchr(p, '#', strlen(p));
            if (commentP != NULL) {
                commentDetected = true;
                break;
            }

            string checkingStringTest = string(p);

            if (checkingStringTest.compare("test") == 0) {
                p = strtok(NULL, " ");
                Test *c = new Test();


                char *checkFlag = (char *) memchr(p, '-', strlen(p));
                if (checkFlag != NULL) {
                    c -> add_flag(p);
                    p = strtok(NULL, " ");
                    c -> add_flag(p);
                    p = strtok(NULL, " ");
                }
                else {
                    c -> add_flag(p);
                    p = strtok(NULL, " ");
                }
                commandList.push(c);

                if (p != 0) {
                    bool checkingConnectors = checkAllCon(p);
                    char *commentP = (char *) memchr(p, '#', strlen(p));

                    if (commentP != NULL) {
                        checkingComment = commentP;
                    }
                    else if (checkingConnectors) {
                        cout << "Error: Expected a connector, Received: " << p << endl;
                        exit(1);
                    }
                    else {
                        connectorList.push(p);
                        p = strtok(NULL, " ");
                    }
                }
                else {
                    break;
                }
            }

            char *testingTest = (char *) memchr(p, '[', strlen(p));
            if (testingTest != NULL) {
                Test *c = new Test();

				p = strtok(NULL, " ");

				for (unsigned i = 0; i < 2; i++) {
				    c -> add_flag(p);
				    p = strtok(NULL, " ");
				}

                if (p != 0) {
                    char *checkingEndTest = (char *) memchr(p, ']', strlen(p));
				    if (checkingEndTest == NULL) {
				        cout << "Error: Expected: ']', Received: " << p << endl;
				        exit(1);
				    }
				}
				
				p = strtok(NULL, " ");
				commandList.push(c);

				if (p != 0) {
				    bool checkingConnectors = checkAllCon(p);
				    if (checkingConnectors) {
				        cout << "Error: Expected a connector, Received: " << p << endl;
				    }
				    char *commentP = (char *) memchr(p, '#', strlen(p));
				    if (commentP != NULL) {
				        checkingComment = commentP;
				    }
				    else {
				        connectorList.push(p);
				        p = strtok(NULL, " ");
				    }
				}
                else {
                    break;
                }
            }

            Cmd *testingCommand = new Cmd(p);
            char *q = p;

            if (!firstArgSemi && !commentDetected) {
                q = strtok(NULL, " ");

                while (q != 0 && !commentDetected) {
                    bool checkConnectors = checkCon(q);

                    char *comment = (char *) memchr(q, '#', strlen(q));
                    if (comment != NULL) {
                        commentDetected = true;
                    }
                    else {
                        if (checkConnectors) {
                            char *semi = (char *) memchr(q, ';', strlen(q));
                            char *com = (char *) memchr(q, '#', strlen(q));

                            if (semi != NULL) {
                                string tempQ = string(q);
                                tempQ = tempQ.substr(0, tempQ.size()-1);
                                strcpy(q, tempQ.c_str());

                                testingCommand->add_flag(q);

                                string Colon = ";";
                                char *pushColon = new char[2];
                                strcpy(pushColon, Colon.c_str());
                                connectorList.push(pushColon);
                                break;
                            }
                            
                            if (com != NULL) {
                                cout << "# found!" << endl;
                                commentDetected = true;
                                break;
                            }

                            else {
                                testingCommand->add_flag(q);
                            }
                        }
                        else {
                            char *com1 = (char *) memchr(q, '#', strlen(q));
                            if (com1 != NULL) {
                                cout << "# found!" << endl;
                                commentDetected = true;
                                break;
                            }
                            if (!checkConnectors) {
                                connectorList.push(q);
                            }
                            break;
                        }
                    }
                    q = strtok(NULL, " ");
                }
                
                p = q;
                p = strtok(NULL, " ");
                
                if (testingCommand -> get_data() == "exit") {
                    Exit *out = new Exit();
                    commandList.push(out);
                }
                else {
                    commandList.push(testingCommand);
                }
            }
            else {
                commandList.push(testingCommand);
                p = q;
                p = strtok(NULL, " ");
                firstArgSemi = false;
            }
        }

        if (connectorList.size() > 0) {
            
            Base *lhs = commandList.front();
            commandList.pop();
            Base *rhs = commandList.front();
            commandList.pop();

            
            char *temp = connectorList.front();
            connectorList.pop();

            if (temp == AND_STRING) {
                AND *n = new AND(lhs, rhs);
                completedListToRun.push(n);
            }
            else if (temp == OR_STRING) {
                OR *n = new OR(lhs, rhs);
                completedListToRun.push(n);
            }
            else if (temp == SEMI_STRING) {
                Semicolon *n = new Semicolon(lhs, rhs);
                completedListToRun.push(n);
            }
            else if (temp == INPUT_RED) {
                InputRed *n = new InputRed(lhs, rhs);
                completedListToRun.push(n);
            }
            else if (temp == SINGLE_OUTPUT_RED) {
                SingleOutRed *n = new SingleOutRed(lhs, rhs);
                completedListToRun.push(n);
            }
            else if (temp == DOUBLE_OUTPUT_RED) {
                DoubleOutRed *n = new DoubleOutRed(lhs, rhs);
                completedListToRun.push(n);
            }
            else if (temp == PIPE) {
                Pipe *n = new Pipe(lhs, rhs);
                completedListToRun.push(n);
            }

            while (connectorList.size() != 0) {
                Connector *tempLHS = completedListToRun.front();
                completedListToRun.pop();
                Base *rhs = commandList.front();
                commandList.pop();

                char *temp2 = connectorList.front();
                connectorList.pop();
                
                if (temp2 == AND_STRING) {
                    AND *n = new AND(tempLHS, rhs);
                    completedListToRun.push(n);
                }
                else if (temp2 == OR_STRING) {
                    OR *n = new OR(tempLHS, rhs);
                    completedListToRun.push(n);
                }
                else if (temp2 == SEMI_STRING) {
                    Semicolon *n = new Semicolon(tempLHS, rhs);
                    completedListToRun.push(n);
                }
                else if (temp2 == INPUT_RED) {
                    InputRed *n = new InputRed(tempLHS, rhs);
                    completedListToRun.push(n);
                }
                else if (temp2 == SINGLE_OUTPUT_RED) {
                    SingleOutRed *n = new SingleOutRed(tempLHS, rhs);
                    completedListToRun.push(n);
                }
                else if (temp2 == DOUBLE_OUTPUT_RED) {
                    DoubleOutRed *n = new DoubleOutRed(tempLHS, rhs);
                    completedListToRun.push(n);
                }
                else if (temp2 == PIPE) {
                    Pipe *n = new Pipe(tempLHS, rhs);
                    completedListToRun.push(n);
                }
            }
            
            Connector *singleRun = completedListToRun.front();
            completedListToRun.pop();
            return singleRun;
        }
        else {
            if (commandList.size() != 1) {
                cout << "Error commandList has more than 1 Cmd*" << endl;
                exit(1);
            }
            else {
                Base *temporaryCmd = commandList.front();
                commandList.pop();
                return temporaryCmd;
            }
        }
    }
    if (commandList.size() > 1) {
        cout << "Error commandList has more than 1 Cmd*" << endl;
        exit(1);
    }
    
    else {
        Base* resultCmd = commandList.front();
        commandList.pop();
        return resultCmd;
    }
    
    return NULL;
    
}

int main(int argc, char**argv) {

    //infinite for loop. break when exit is found
    for (; ;) {
        char *userName = getlogin(); 
        if (!userName) {
            perror("getlogin() error");
        }

        char hostName[1000];
        gethostname(hostName, sizeof hostName);

        cout << userName << "@" <<  hostName << "$ ";
        //End of login info

        string userInput;
        getline(cin, userInput);

        if (userInput == "exit") {
            Exit *exit = new Exit();
            exit->execute(0,1);
            break;
        }

        char *cstr = new char[userInput.size()+1];
        strcpy(cstr, userInput.c_str()); 

//precendence
        queue<Base *> precedenceTrees;
        queue<Connector *> outsideConnectors;

        size_t foundPrecedence = userInput.find('(');
        size_t foundTest = userInput.find('[');
        size_t foundOutput = userInput.find('>');
        size_t foundInput = userInput.find('<');
        size_t foundPipe = userInput.find('|');

        if (foundPrecedence!=std::string::npos || (foundPrecedence!=std::string::npos && foundTest!=std::string::npos) || (foundPrecedence!=std::string::npos && (foundOutput!=std::string::npos && foundInput!=std::string::npos && foundPipe!=std::string::npos))) { 
            string totalString = "";

            char *p = strtok(cstr, " ");

            while (p!=0) {
                char *checkingPrecedenceF = (char *) memchr(p, '(', strlen(p));
                char *checkingPrecedenceE = (char *) memchr(p, ')', strlen(p));
                char *checkingTestB = (char *) memchr(p, '[', strlen(p));
                char *checkingTestE = (char *) memchr(p, ']', strlen(p));
                char *checkingInput = (char *) memchr(p, '<', strlen(p));
                char *checkingPipe = (char *) memchr(p, '|', strlen(p));
                char *checkingOutput = (char *) memchr(p, '>', strlen(p));

                int totalEndingPrecedence = 0;

                if (checkingPrecedenceF != NULL && (checkingInput != NULL || checkingPipe != NULL || checkingOutput!= NULL)) {
                    while (checkingPrecedenceF != NULL) {
                        totalString += "( ";
                        splitUpFirstCharacter(p);
                        checkingPrecedenceF = (char *) memchr(p, '(', strlen(p));
                    }
                    string pString = SepCommands(p);

                    totalString += pString;
                    while (checkingPrecedenceE != NULL) {
                        totalEndingPrecedence++;
                        splitUpLastCharacter(p);
                        checkingPrecedenceE = (char *) memchr(p, ')', strlen(p));
                    }

                    char *checkingComma = (char *) memchr(p, ';', strlen(p));
                    char *checkingComment = (char *) memchr(p, '#', strlen(p));

                    if (checkingComma != NULL || checkingComment != NULL) {
                        splitUpLastCharacterAlways(p);
                        totalString += string(p);
                        totalString += " ";
                        for (int i=0; i<totalEndingPrecedence; i++) {
                            totalString += ") ";
                        }
                        if (checkingComma != NULL) {
                            totalString += "; ";
                        }
                    }
                    
                    else {
                        for (int i=0; i<totalEndingPrecedence; i++) {
                            totalString += " ) ";
                        }
                    }
                }
                
                else if (checkingPrecedenceF != NULL && checkingTestB != NULL && checkingTestE != NULL) {
                    while (checkingPrecedenceF != NULL) {
                        totalString += "( ";
                        splitUpFirstCharacter(p);
                        checkingPrecedenceF = (char *) memchr(p, '(', strlen(p));
                    }

                    totalString += "[ ";
                    splitUpFirstCharacter(p);

                    while (checkingPrecedenceE != NULL) {
                        totalEndingPrecedence++;
                        splitUpLastCharacter(p);
                        checkingPrecedenceE = (char *) memchr(p, ')', strlen(p));
                    }

                    char *checkingComma = (char *) memchr(p, ';', strlen(p));
                    char *checkingComment = (char *) memchr(p, '#', strlen(p));

                    if (checkingComma != NULL || checkingComment != NULL) {
                        splitUpLastCharacterAlways(p);
                        totalString += string(p);
                        totalString += " ";
                        for (int i=0; i<totalEndingPrecedence; i++) {
                            totalString += ") ";
                        }
                        if (checkingComma != NULL) {
                            totalString += "; ";
                        }
                        else {
                            totalString += "# ";
                        }
                    }
                    else {
                        splitUpLastCharacterAlways(p);
                        totalString += string(p);
                        totalString += " ";
                        totalString += "] ";
                        for (int i=0; i<totalEndingPrecedence; i++) {
                            totalString += ") ";
                        }
                    }

                }
                else if (checkingInput != NULL || checkingOutput != NULL || checkingPipe != NULL) {
                    string pString = SepCommands(p);

                    totalString += pString;

                    while (checkingPrecedenceE != NULL) {
                        totalEndingPrecedence++;
                        splitUpLastCharacter(p);
                        checkingPrecedenceE = (char *) memchr(p, ')', strlen(p));
                    }

                    char *checkingComma = (char *) memchr(p, ';', strlen(p));
                    char *checkingComment = (char *) memchr(p, '#', strlen(p));

                    if (checkingComma != NULL || checkingComment != NULL) {
                        splitUpLastCharacterAlways(p);
                        totalString += string(p);
                        totalString += " ";
                        for (int i=0; i<totalEndingPrecedence; i++) {
                            totalString += ") ";
                        }
                        if (checkingComma != NULL) {
                            totalString += "; ";
                        }
                    }
                    else {
                        for (int i=0; i<totalEndingPrecedence; i++) {
                            totalString += " ) ";
                        }
                    }
                }
                else if (checkingPrecedenceF != NULL && checkingTestB != NULL) {
                    while (checkingPrecedenceF != NULL) {
                        totalString += "( ";
                        checkingPrecedenceF = (char *) memchr(p, '(', strlen(p));
                        splitUpFirstCharacter(p);
                    }
                    totalString += "[ ";
                    totalString += string(p);
                    totalString += " ";
                }
                else if (checkingTestB != NULL && checkingTestE != NULL) {
                    totalString += "[ ";
                    splitUpFirstCharacter(p);
                    splitUpLastCharacter(p);
                    totalString += string(p);
                    totalString += " ";
                    totalString += "] ";
                }
                else if (checkingTestB != NULL) {
                    totalString += "[ ";
                    splitUpFirstCharacter(p);
                    totalString += string(p);
                    totalString += " ";
                }
                else if (checkingTestE != NULL) {
                    splitUpLastCharacter(p);

                    totalString += string(p);
                    totalString += " ";
                    totalString += "] ";
                }
                else if (checkingPrecedenceF != NULL) {
                    while (checkingPrecedenceF != NULL) {
                        totalString += "( ";
                        splitUpFirstCharacter(p);
                        checkingPrecedenceF = (char *) memchr(p, '(', strlen(p));
                    }
                    totalString += string(p);
                    totalString += " ";
                }
                else if (checkingPrecedenceE != NULL) {
                    while (checkingPrecedenceE != NULL) {
                        totalEndingPrecedence++;
                        splitUpLastCharacter(p);
                        checkingPrecedenceE = (char *) memchr(p, ')', strlen(p));
                    }

                    char *checkingComma = (char *) memchr(p, ';', strlen(p));
                    char *checkingComment = (char *) memchr(p, '#', strlen(p));

                    if (checkingComma != NULL || checkingComment != NULL) {
                        splitUpLastCharacterAlways(p);
                        totalString += string(p);
                        totalString += " ";
                        for (int i=0; i<totalEndingPrecedence; i++) {
                            totalString += ") ";
                        }
                        if (checkingComma != NULL) {
                            totalString += "; ";
                        }
                        else {
                            totalString += "# ";
                        }
                    }
                    else {
                        totalString += string(p);
                        totalString += " ";
                        for (int i=0; i<totalEndingPrecedence; i++) {
                            totalString += ") ";
                        }
                    }
                }
                else {
                    totalString += string(p);
                    totalString += " ";
                }

                p = strtok(NULL, " ");
            }

            char *totalChar = new char[totalString.size()+1];
            strcpy(totalChar, totalString.c_str());

            char *c = strtok(totalChar, " ");

	        stack<char *> stringStack;//seperates 2 diff strings
	        queue<string> branches;
	        queue<char *> connectors;

	        bool withinPrecedence = false;                         
	        bool newPrecedence = false;

	        while (c!=0) {
	            char *beginPrecedence = (char *) memchr(c, '(', strlen(c));
	            char *endPrecedence = (char *) memchr(c, ')', strlen(c));
	            bool checkConnectors = checkAllCon(c);     //check if Token is a connector

	            if (beginPrecedence != NULL) {
	                withinPrecedence = true;

	                if (!newPrecedence) {
	                    stack<char *> currentString;

	                    while (!stringStack.empty()) {
	                        char *a = stringStack.top();
	                        char *checkingStartPrecedence = (char *) memchr(a, '(', strlen(a));

	                        if (checkingStartPrecedence != NULL) {
	                            stringStack.pop();
	                            break;
	                        }

	                        currentString.push(a);
	                        stringStack.pop();
	                    }
	                    string finalString;

	                    while (!currentString.empty()) {
	                        finalString += currentString.top();
	                        finalString += " ";
	                        currentString.pop();
	                    }
	                    ////test
	                    //cout << finalString << endl;
	                    if (finalString.size() > 0) {
	                        branches.push(finalString);
	                    }
	                }

	                newPrecedence = true;
	            }
	            else if (endPrecedence != NULL) {
	                withinPrecedence = false;
	                newPrecedence = false;
	                stack<char *> currentString;

	                while (!stringStack.empty()) {
	                    char *a = stringStack.top();
	                    char *checkingStartPrecedence = (char *) memchr(a, '(', strlen(a));

	                    if (checkingStartPrecedence != NULL) {
	                        stringStack.pop();
	                        break;
	                    }

	                    currentString.push(a);
	                    stringStack.pop();
	                }
	                string finalString;

	                while (!currentString.empty()) {
	                    finalString += currentString.top();
	                    finalString += " ";
	                    currentString.pop();
	                }
	                ////test
	                //cout << finalString << endl;
	                if (finalString.size() > 0) {
	                    branches.push(finalString);
	                }
	            }
	            else if (!checkConnectors && !withinPrecedence) {
	                connectors.push(c);
	            }
	            else {
	                stringStack.push(c);
	            }
	            c = strtok(NULL, " ");
	        }

            //reverses order
	        stack<char *> currentOvers;
	        while (!stringStack.empty()) {
	            currentOvers.push(stringStack.top());
	            stringStack.pop();
	        }

	        string leftOvers;
	        while (!currentOvers.empty()) {
	            leftOvers += currentOvers.top();
	            leftOvers += " ";
	            currentOvers.pop();
	        }

	        if (leftOvers.size() != 0) {
	            branches.push(leftOvers);
	        }

            queue<Base *> commandTreeList;

	        while (!branches.empty()) {
	            char *r = new char[branches.front().size()+1];
	            strcpy(r, branches.front().c_str());

	            branches.pop();
	            Base* tree = grabTree(r);
                if (tree != NULL) {
                    commandTreeList.push(tree);
	            }
	        }

            if (connectors.size() > 0) {
                queue<Connector *>completedListToRun;

                Base* lhs = commandTreeList.front();
                commandTreeList.pop();
                Base* rhs = commandTreeList.front();
                commandTreeList.pop();

                char *temp = connectors.front();
                connectors.pop();

                if (temp == AND_STRING) {
                    AND *n = new AND(lhs, rhs);
                    completedListToRun.push(n);
                }
                else if (temp == OR_STRING) {
                    OR *n = new OR(lhs, rhs);
                    completedListToRun.push(n);
                }
                else if (temp == SEMI_STRING) {
                    Semicolon *n = new Semicolon(lhs, rhs);
                    completedListToRun.push(n);
                }
                else if (temp == INPUT_RED) {
                    InputRed *n = new InputRed(lhs, rhs);
                    completedListToRun.push(n);
                }
                else if (temp == SINGLE_OUTPUT_RED) {
                    SingleOutRed *n = new SingleOutRed(lhs, rhs);
                    completedListToRun.push(n);
                }
                else if (temp == DOUBLE_OUTPUT_RED) {
                    DoubleOutRed *n = new DoubleOutRed(lhs, rhs);
                    completedListToRun.push(n);
                }
                else if (temp == PIPE) {
                    Pipe *n = new Pipe(lhs, rhs);
                    completedListToRun.push(n);
                }
                while (connectors.size() > 0) {
                    Connector *tempLHS = completedListToRun.front();
                    completedListToRun.pop();
                    Base *rhs = commandTreeList.front();
                    commandTreeList.pop();

                    char *temp2 = connectors.front();
                    connectors.pop();

                    if (temp2 == AND_STRING) {
                        AND *n = new AND(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                    else if (temp2 == OR_STRING) {
                        OR *n = new OR(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                    else if (temp2 == SEMI_STRING) {
                        Semicolon *n = new Semicolon(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                    else if (temp2 == INPUT_RED) {
                        InputRed *n = new InputRed(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                    else if (temp2 == SINGLE_OUTPUT_RED) {
                        SingleOutRed *n = new SingleOutRed(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                    else if (temp2 == DOUBLE_OUTPUT_RED) {
                        DoubleOutRed *n = new DoubleOutRed(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                    else if (temp2 == PIPE) {
                        Pipe *n = new Pipe(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                }


                Connector *singleRun = completedListToRun.front();
                completedListToRun.pop();
                singleRun->execute(0,1);
           }
           else {
                Base* s = commandTreeList.front();
                commandTreeList.pop();

                if (commandTreeList.size() != 0) {
                    cout << "Error: commandTreeList.size() != 0" << endl;
                    exit(1);
                }
                s->execute(0,1);
            }
        }
        else {
            size_t foundTest = userInput.find('[');
            if (foundTest!=std::string::npos) {
                string totalString = "";
                char *p = strtok(cstr, " ");
                while (p!=0) {
                    char *checkingTestB = (char *) memchr(p, '[', strlen(p));
                    char *checkingTestE = (char *) memchr(p, ']', strlen(p));

                    if (checkingTestB != NULL && checkingTestE != NULL) {
                        totalString += "[ ";
                        splitUpFirstCharacter(p);
                        splitUpLastCharacter(p);
                        totalString += string(p);
                        totalString += " ";
                        totalString += "] ";
                    }
                    else if (checkingTestB != NULL) {
                        totalString += "[ ";
                        splitUpFirstCharacter(p);
                        totalString += string(p);
                        totalString += " ";
                    }
                    else if (checkingTestE != NULL) {
                        splitUpLastCharacter(p);
                        totalString += string(p);
                        totalString += " ";
                        totalString += "] ";
                    }
                    else {
                        totalString += string(p);
                        totalString += " ";
                    }
                    p = strtok(NULL, " ");
                }

	            char *r = new char[totalString.size()+1];
	            strcpy(r, totalString.c_str());

                Base *s = grabTree(r);
                s->execute(0,1);
            }
            else {
                Base* s = grabTree(cstr);
                s->execute(0,1);
            }
        }

        delete[] cstr;  //deallo mem
    }
    return 0;
}
