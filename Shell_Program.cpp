/*

Purpose:
The purpose of this project is to familiarize you with the mechanics of process control through the
implementation of a shell user interface. This includes the relationship between child and parent
processes, the steps needed to create a new process, shell variables, and an introduction to userinput parsing and verification.

Your shell program must be able to run on a Linux environment such as Ubuntu on Windows
Linux Subsystem.

Problem Statement:
Design and implement a basic shell interface that supports the execution of other programs and a
series of built-in functions, as specified below. The shell should be robust (e.g., it should not crash
under any circumstance beyond machine failure). 

*/

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
