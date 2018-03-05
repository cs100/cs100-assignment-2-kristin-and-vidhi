#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <queue>
#include <stack>

using namespace std;

#include "Base.h"
#include "Cmd.cpp"
#include "Connector.cpp"
#include "AND.cpp"
#include "OR.cpp"
#include "Semicolon.cpp"
#include "Exit.cpp"
#include "Test.cpp"

const string AND_STRING = "&&";
const string OR_STRING = "||";
const string SEMI_STRING = ";";

void splitUpFirstCharacter(char* p) {

    string tempP = string(p);
    tempP = tempP.substr(1, tempP.size()-1);
    strcpy(p, tempP.c_str());
}

void splitUpLastCharacter(char* p) {
    string tempP = string(p);
    size_t comments = tempP.find('#');
    size_t comma = tempP.find(';');
    if (comma!=std::string::npos || comments!=std::string::npos) {
        tempP.erase(tempP.end()-2);
    }
    else {
        tempP.erase(tempP.end()-1);
    }
    strcpy(p, tempP.c_str());
}

void splitUpLastCharacterAlways(char *p) {
    string tempP = string(p);
    tempP.erase(tempP.end()-1);
    strcpy(p, tempP.c_str());
}

bool checkCon(char *q) {
    vector<string> s;
    s.push_back("&&");
    s.push_back("||");

    for (unsigned i=0; i<s.size(); i++)
        if (q == s.at(i))
            return false;
    return true;
}

//Checks all the connectors
bool checkAllCon(char *q) {
    vector<string> s;
    s.push_back("&&");
    s.push_back("||");
    s.push_back("#");
    s.push_back(";");

    for (unsigned i=0; i<s.size(); i++)
        if (q == s.at(i))
            return false;
    return true;
}

Base* grabTree(char *cstr) {

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
        tempP = tempP.substr(0, tempP.size()-1);
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
            c->add_flag(p);
            p = strtok(NULL, " ");
            c->add_flag(p);
            p = strtok(NULL, " ");
        }
        else {                 
            c->add_flag(p);
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
            c->add_flag(p);
            p = strtok(NULL, " ");
            c->add_flag(p);
            p = strtok(NULL, " ");
        }
        else {
            c->add_flag(p);
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

        if (p!=0) {
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
        while (p!=0 && !commentDetected) {
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
                    c->add_flag(p);
                    p = strtok(NULL, " ");
                    c->add_flag(p);
                    p = strtok(NULL, " ");
                }
                else {
                    c->add_flag(p);
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

            char *testingTest = (char *) memchr(p, '[', strlen(p)); //Same as top
            if (testingTest != NULL) {
                Test *c = new Test();

				p = strtok(NULL, " ");

				for (unsigned i=0; i<2; i++) {
				    //cout << p << endl;
				    c->add_flag(p);
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
				//cout << p << endl;
				commandList.push(c);

				if (p!=0) {
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

                while (q!=0 && !commentDetected) {
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
                }//End while

                p = q;               
                p = strtok(NULL, " "); 

                if (testingCommand->getCommand() == "exit") {  
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

        if (connectorList.size() > 0) { // runs when 2 or more commands
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
            if (temp == OR_STRING) {
                OR *n = new OR(lhs, rhs);
                completedListToRun.push(n);
            }
            if (temp == SEMI_STRING) {
                Semicolon *n = new Semicolon(lhs, rhs);
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

    
    for (; ;) {
        char *userName = getlogin();            
        if (!userName) {
            perror("getlogin() error");
        }

        char hostName[1000];
        gethostname(hostName, sizeof hostName); 

        cout << userName << "@" <<  hostName << "$ ";                          
//end login

        string userInput;                       
        getline(cin, userInput);

        if (userInput == "exit") {
            Exit *exit = new Exit();
            exit->execute();
            break;
        }

        char *cstr = new char[userInput.size()+1];                 
        strcpy(cstr, userInput.c_str());
        queue<Base *> precedenceTrees;
        queue<Connector *> outsideConnectors;

        size_t foundPrecedence = userInput.find('(');              
        size_t foundTest = userInput.find('[');                    
        if (foundPrecedence!=std::string::npos || (foundPrecedence!=std::string::npos && foundTest!=std::string::npos)) {   
            string totalString = "";

            char *p = strtok(cstr, " ");

            while (p!=0) {
                char *checkingPrecedenceF = (char *) memchr(p, '(', strlen(p));
                char *checkingPrecedenceE = (char *) memchr(p, ')', strlen(p));
                char *checkingTestB = (char *) memchr(p, '[', strlen(p));
                char *checkingTestE = (char *) memchr(p, ']', strlen(p));
                int totalEndingPrecedence = 0;

                if (checkingPrecedenceF != NULL && checkingPrecedenceF != NULL && checkingTestB != NULL && checkingTestE != NULL) {
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

	        stack<char *> stringStack;                           
	        queue<string> branches;
	        queue<char *> connectors;

	        bool withinPrecedence = false;                    
	        bool newPrecedence = false;                             
	       
	        while (c!=0) {
	            char *beginPrecedence = (char *) memchr(c, '(', strlen(c));
	            char *endPrecedence = (char *) memchr(c, ')', strlen(c));
	            bool checkConnectors = checkAllCon(c); 

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
                }

                Connector *singleRun = completedListToRun.front();
                completedListToRun.pop();
                singleRun->execute();
           }
           else {
                Base* s = commandTreeList.front();
                commandTreeList.pop();

                if (commandTreeList.size() != 0) {
                    cout << "Error: commandTreeList.size() != 0" << endl;
                    exit(1);
                }

                s->execute();
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
                s->execute();
            }
            else {
                Base* s = grabTree(cstr);
                s->execute();
            }
        }

        delete[] cstr;  //deallo memory
    }
    return 0;
}
