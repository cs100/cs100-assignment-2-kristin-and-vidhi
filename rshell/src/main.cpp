#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <queue>
#include <cstdlib>

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "Base.h"
#include "Cmd.cpp"
#include "Connector.cpp"
#include "AND.cpp"
#include "OR.cpp"
#include "Semicolon.cpp"
#include "Exit.cpp"

using namespace std;

//pass char and return if isConnector is connector or not. 
bool checkCon (char *isConnector) {
    vector<string> s;
    s.push_back ("&&");
    s.push_back ("||");
    for (unsigned i = 0; i < s.size (); i++) {
        if (isConnector == s.at (i)) {
            return false;
        }
    }
    return true;
}

int main (int argc, char**argv) {

//KRISTIN! this exits when user puts exit in terminal
    for (; ;) {
        queue<Cmd* > commandList; //fro commands
        queue<char *> connectorList;  //for connectors
        queue<Connector *> completedListToRun;
                                        
                                        
        //to display user's name and machine name. 
        //I just did this bc i had it. it looks cool lol. 
        char *userName = getlogin (); 
        if (!userName) {
            perror ("getlogin () error");
        }

        char hostName[1000];
        gethostname (hostName, sizeof hostName);

        cout << userName << "@" <<  hostName << "$ ";

        string userInput;
        getline (cin, userInput);

//if user puts exit command
        if (userInput == "exit") {
            Exit *exit = new Exit ();
            exit->execute ();
            break;
        }

        char *cstr = new char[userInput.size ()+1]; 
        strcpy (cstr, userInput.c_str ());

        char *p = strtok (cstr, " ");

        bool firstArgSemi = false;
        char *checkingSemi =  (char *) memchr (p, ';', strlen (p));
        char *checkingComment =  (char *) memchr (p, '#', strlen (p));


//if theres a semicolon. erase it and put bool to true? then push to str
//**try to redo the push part. 
        if (checkingSemi != NULL) {                                        
            string tempP = string (p);
            tempP = tempP.substr (0, tempP.size ()-1);
            strcpy (p, tempP.c_str ());

            string semiCol = ";";
            char *pushConnector = new char[2];
            strcpy (pushConnector, semiCol.c_str ());
            connectorList.push (pushConnector);

           // firstArgSemi = false;
            firstArgSemi = true;
        }

        if (checkingComment != NULL) {

        }
        else {
            bool commentDetected = false;
            while (p!=0 && !commentDetected) {
                char *commentP =  (char *) memchr (p, '#', strlen (p));
                if (commentP != NULL) {
                    commentDetected = true;
                    break;
                }

                Cmd *testingCommand = new Cmd (p);
                char *q = p;

            if (!firstArgSemi && !commentDetected) {
                q = strtok (NULL, " "); 

                while (q!=0 && !commentDetected) {
                    bool checkConnectors = checkCon (q);

                        //check for comments
                    char *comment =  (char *) memchr (q, '#', strlen (q));
                    if (comment != NULL) {
                        commentDetected = true;
                    }
                    
                    else {
                        if (checkConnectors) {
                            char *semi =  (char *) memchr (q, ';', strlen (q));
                            char *com =  (char *) memchr (q, '#', strlen (q));

                            //If so do the same algorithm except add the flag ';' to Cmd testingCommand
                                
                                
                            //  if (com != NULL) {
                            //     cout << "# found!" << endl;
                            //     commentDetected = true;
                            //     break;
                            // }
                                
                            if (semi != NULL) {
                            string tempQ = string (q);
                            tempQ = tempQ.substr (0, tempQ.size ()-1);
                            strcpy (q, tempQ.c_str ());

                            testingCommand->add_flag (q);

                            string Colon = ";";
                            char *pushColon = new char[2];
                            strcpy (pushColon, Colon.c_str ());
                            connectorList.push (pushColon);
                            break;
                            }
                                
                            if (com != NULL) {
                                cout << "# found!" << endl;
                                commentDetected = true;
                                break;
                            }
                                
                            else {
                                testingCommand->add_flag (q);
                            }
                        }
                            
                        else {
                            char *com1 =  (char *) memchr (q, '#', strlen (q));
                            if (com1 != NULL) {
                                cout << "# found!" << endl;
                                commentDetected = true;
                                break;
                            }

                            if (!checkConnectors) {
                                connectorList.push (q);
                            }

                            break;
                        }
                    }
                    q = strtok (NULL, " ");
                }
                    //end while loop
                    //this should have checked for connectors, for semicolons, for comments

                p = q;
                p = strtok(NULL, " ");

                if (testingCommand->getCommand () == "exit") {   
                    Exit *out = new Exit (); 
                    commandList.push (out);
                }
                    
                else {
                    commandList.push (testingCommand);
                }
            }
                
            else {
                commandList.push (testingCommand);
                p = q; 
                p = strtok (NULL, " ");
                firstArgSemi = false;                       
            }
        }


//*********************************************IF <2 COMMANDSS

            if (connectorList.size () > 0) {
                string AND_STRING = "&&";
                string OR_STRING = "||";
                string SEMI_STRING = ";";


                //  Cmd *rhs = commandList.front ();
                //commandList.pop ();
                Cmd *lhs = commandList.front ();
                commandList.pop ();
                Cmd *rhs = commandList.front ();
                commandList.pop ();

                //pop connector
                char *temp = connectorList.front ();
                connectorList.pop ();

                //check connector and push it to the array
                if (temp == AND_STRING) {
                    AND *n = new AND (lhs, rhs);
                    completedListToRun.push (n);
                }
                if (temp == OR_STRING) {
                    OR *n = new OR (lhs, rhs);
                    completedListToRun.push (n);
                }
                if (temp == SEMI_STRING) {
                    Semicolon *n = new Semicolon (lhs, rhs);
                    completedListToRun.push (n);
                }



                while (connectorList.size () != 0) {
                    Connector *tempLHS = completedListToRun.front ();
                    completedListToRun.pop ();
                    Cmd *rhs = commandList.front ();
                    commandList.pop ();

                    char *temp2 = connectorList.front ();
                    connectorList.pop ();
                    if (temp2 == AND_STRING) {
                        AND *n = new AND (tempLHS, rhs);
                        completedListToRun.push (n);
                    }
                    else if (temp2 == OR_STRING) {
                        OR *n = new OR (tempLHS, rhs);
                        completedListToRun.push (n);
                    }
                    else if (temp2 == SEMI_STRING) {
                        Semicolon *n = new Semicolon (tempLHS, rhs);
                        completedListToRun.push (n);
                    }

                }
                
                //pop command, run execute on that popped command. execvp will do the individual command objects respectively. 
                Connector *singleRun = completedListToRun.front ();
                completedListToRun.pop ();
                singleRun->execute ();

            }
            else {  //no connectors
            //checking if one command
                if (commandList.size () != 1) {
                    cout << "Error commandList has more than 1 Cmd*" << endl;
                    return 1;
                }
                else {
                    //pop and execvp
                    Cmd *temporaryCmd = commandList.front ();
                    commandList.pop ();
                    temporaryCmd->execute ();
                }
            }
        }//End of infinite for loop

        delete[] cstr;
        //delete so that no mem leaks
    }
    return 0;
}
