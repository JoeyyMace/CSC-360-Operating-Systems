#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>

using namespace std;

void parse_args(string line, vector<string> &cmds){
    stringstream liness(line);
 
    string token;
    while (getline(liness, token, ' ')) {
        cmds.push_back(token);
    }
}

int main(void)
{
    int argc = 0;
    while (1)
    {
        // prompt
        cout << "lsmsh$ ";

        string cmd;
        getline(cin, cmd);

        // ignore empty input
        if (cmd == "" || cmd.size() == 0) 
            continue;

        cout <<"Received user commands: ";
        cout << cmd << endl;
        
        // built-in: exit
        if (cmd == "help"){
            cout << "Please print your help message of this program.\n";
            continue;
        }else if(cmd == "exit"){
            exit(0);
        }

        vector<string> cmd_args;
        parse_args(cmd, cmd_args);

        // fork child and execute program
        int pid = fork();
        int status;
        if (pid == 0)
        {
            char * argv[4] = {"ls", "-l", "-a", NULL};
            execvp(argv[0], argv);
        }
        else
        {
            // wait for program to finish and print exit status
            waitpid(pid, &status, 0);
        }
    }
}
