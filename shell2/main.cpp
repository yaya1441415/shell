#include <iostream>
#include <cstring>


#include "windows.h"
#include  "direct.h"

#define MAXLIST 100 //max number of commands to be supported.
#define MAXCOM 1000 //max number of letters to be supported.
#define clear() printf("\033[H\033[J")

/**
 * init_shell() function initializes the shell
 * environment by displaying a welcome message
 * and the current user's name.
 */

void init_shell(){
    //clear terminal screen.
    clear();
    //display a welcome banner.

    printf("\n\n\n\n******************"
           "************************");
    printf("\n\n\n\t****MY SHELL****");
    printf("\n\n\t-USE AT YOUR OWN RISK-");
    printf("\n\n\n\n*******************"
           "***********************");

    //Retreives and display Username.
    char* username = getenv("USERNAME");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");

    //pauses the program 1000 milliseconds.
    Sleep(1000);

    clear();
}

int takeInput(char* str){
    // Get input from user and store it in a string name buf.
    std::string buf;
    std::cout<<"\n>>> ";
    std::getline(std::cin,buf);

    //check if the input is not empty.
    if (buf.size()!=0){
        //copy input to the parameter.
        strcpy(str, buf.c_str());
        return 0;
    }else{
        return 1;
    }
}

void printDir(){
    char cwd[1024];
    if (GetCurrentDirectory(sizeof(cwd), cwd)==0){
        std::cerr<<"Error getting current directory."<<std::endl;

    }
    else{
        printf("\nDir: %s", cwd);
    }

}


/**
 * execArgs function is responsible for executing a command
 * ina new process. it takes an array of command arguments,
 * turns it into a string, then uses windows API "CreateProcess()"
 * to create and run the command in a new process.
 *
 *
 * @param parsed
 */
//void execArgs(char** parsed){
//    // convert parsed command array into a single command string,
//    //because CreateProcess expects a single command line string.
//    std::string command;
//    for (int i = 0; parsed[i]!=NULL ; ++i) {
//        command+=parsed[i];
//        command+=" ";
//    }
//
//    STARTUPINFO si;
//    PROCESS_INFORMATION pi;
//    ZeroMemory(&si, sizeof(si));
//    si.cb = sizeof(si);
//    ZeroMemory(&pi, sizeof(pi));
//
//    //Create the child process
//    if(!CreateProcess(
//            NULL,
//            const_cast<char*>(command.c_str()),
//            NULL,
//            NULL,
//            FALSE,
//            0,
//            NULL,
//            NULL,
//            &si,
//            &pi
//            )){
//        std::cerr<<"Failed to create process. Error: "<<GetLastError()<<std::endl;
//        return;
//
//    }
//    //wait until child process exits.
//    WaitForSingleObject(pi.hProcess, INFINITE);
//
//    CloseHandle(pi.hProcess);
//    CloseHandle(pi.hThread);
//
//}

void execArgs(char** parsed) {
    std::string command = "cmd /c ";
    for (int i = 0; parsed[i] != NULL; ++i) {
        command += parsed[i];
        command += " ";
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Create the child process using the command
    if (!CreateProcess(
            NULL,
            const_cast<char*>(command.c_str()),  // Command string
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi)) {
        std::cerr << "Failed to create process. Error: " << GetLastError() << std::endl;
        return;
    }

    // Wait until child process exits
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}


//void execArgsPiped(char** parsed, char** parsedpipe){
//    HANDLE pipeRead, pipeWrite;
//    PROCESS_INFORMATION procInfo1, procInfo2;
//    SECURITY_ATTRIBUTES sa;
//    STARTUPINFO startInfo1, startInfo2;
//    BOOL success;
//
//    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
//    sa.bInheritHandle = TRUE;
//    sa.lpSecurityDescriptor = NULL;
//
//    if(!CreatePipe(&pipeRead, &pipeWrite, &sa, 0)){
//        printf("Pipe could not be creted.\n");
//        return;
//    }
//    //Create the first child process
//    ZeroMemory(&procInfo1, sizeof(PROCESS_INFORMATION));
//    ZeroMemory(&startInfo1, sizeof(STARTUPINFO));
//    startInfo1.cb = sizeof(STARTUPINFO);
//    startInfo1.hStdOutput = pipeWrite;
//    startInfo1.hStdError = pipeWrite;
//    startInfo1.dwFlags |= STARTF_USESTDHANDLES;
//
//    success = CreateProcess(NULL, parsed[0], NULL, NULL, TRUE,0,NULL, NULL, &startInfo1, &procInfo1);
//    if(!success){
//        printf("Could not create process 1.\n");
//        CloseHandle(pipeRead);
//        CloseHandle(pipeWrite);
//        return;
//    }
//    ZeroMemory(&procInfo2, sizeof(PROCESS_INFORMATION));
//    ZeroMemory(&startInfo2, sizeof(STARTUPINFO));
//    startInfo2.cb = sizeof(STARTUPINFO);
//    startInfo2.hStdInput = pipeRead;
//    startInfo2.hStdError = GetStdHandle(STD_ERROR_HANDLE);
//    startInfo2.dwFlags |= STARTF_USESTDHANDLES;
//
//
//    success = CreateProcess(NULL,  parsedpipe[0], NULL, NULL, TRUE, 0, NULL, NULL, &startInfo2, &procInfo2);
//
//    if(!success){
//        printf("Could not create a process 2.\n");
//        CloseHandle(pipeRead);
//        CloseHandle(pipeWrite);
//        CloseHandle(procInfo1.hProcess);
//        CloseHandle(procInfo1.hThread);
//
//    }
//
//    CloseHandle(pipeRead);
//    CloseHandle(pipeWrite);
//    CloseHandle(procInfo1.hProcess);
//    CloseHandle(procInfo1.hThread);
//    CloseHandle(procInfo2.hProcess);
//    CloseHandle(procInfo2.hThread);
//
//    // Wait for both child processes to complete
//    WaitForSingleObject(procInfo1.hProcess, INFINITE);
//    WaitForSingleObject(procInfo2.hProcess, INFINITE);
//}

void execArgsPiped(char** parsed, char** parsedpipe) {
    HANDLE pipeRead, pipeWrite;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&pipeRead, &pipeWrite, &sa, 0)) {
        std::cerr << "Pipe could not be created." << std::endl;
        return;
    }

    STARTUPINFO si1, si2;
    PROCESS_INFORMATION pi1, pi2;
    ZeroMemory(&si1, sizeof(si1));
    ZeroMemory(&si2, sizeof(si2));
    si1.cb = sizeof(si1);
    si2.cb = sizeof(si2);
    si1.hStdOutput = pipeWrite;
    si1.dwFlags |= STARTF_USESTDHANDLES;
    si2.hStdInput = pipeRead;
    si2.dwFlags |= STARTF_USESTDHANDLES;

    std::string cmd1 = "cmd /c ";
    for (int i = 0; parsed[i] != NULL; ++i) {
        cmd1 += parsed[i];
        cmd1 += " ";
    }

    std::string cmd2 = "cmd /c ";
    for (int i = 0; parsedpipe[i] != NULL; ++i) {
        cmd2 += parsedpipe[i];
        cmd2 += " ";
    }

    if (!CreateProcess(NULL, const_cast<char*>(cmd1.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si1, &pi1)) {
        std::cerr << "Failed to create process 1. Error: " << GetLastError() << std::endl;
        CloseHandle(pipeRead);
        CloseHandle(pipeWrite);
        return;
    }

    if (!CreateProcess(NULL, const_cast<char*>(cmd2.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2)) {
        std::cerr << "Failed to create process 2. Error: " << GetLastError() << std::endl;
        CloseHandle(pipeRead);
        CloseHandle(pipeWrite);
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        return;
    }

    // Close pipes that are not needed
    CloseHandle(pipeWrite);
    CloseHandle(pipeRead);

    // Wait for both processes to complete
    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);
}

void openHelp(){
    puts("\n***WELCOME TO MY SHELL HELP***"
         "\nCopyright @ Suprotik Dey"
         "\n-Use the shell at your own risk..."
         "\nList of Commands supported:"
         "\n>cd"
         "\n>ls"
         "\n>exit"
         "\n>all other general commands available in windows shell"
         "\n>pipe handling"
         "\n>improper space handling");

    return;
}

int ownCmdHandler(char** parsed){
    int NoOfOwnCmds=4,switchOwnArg = 0;
    const char* ListofOwnCmds[NoOfOwnCmds];
    const char * username;

    ListofOwnCmds[0] = "exit";
    ListofOwnCmds[1] = "cd";
    ListofOwnCmds[2] = "help";
    ListofOwnCmds[3] = "hello";

    for (int i = 0; i < NoOfOwnCmds; ++i) {
        if (strcmp(parsed[0], ListofOwnCmds[i]) == 0){
            switchOwnArg = i+1;
            break;
        }
    }
    switch(switchOwnArg){
        case 1:
            printf("\nGoodbye\n");
            exit(0);

        case 2:
            chdir(parsed[1]);
            return 1;
        case 3:
            openHelp();
            return 1;

        case 4:
            username = getenv("USERNAME");
            printf("\nHello %s.\nMind that this is "
                   "not a place to play around."
                   "\nUse help to know more..\n",
                   username);
            return 1;

            default:
                break;


    }
    return 0;
}

//parse a command string and split it into separate
// commands based on the presence of a pipe "|".
//return 1 if a pipe is found nd the string is
//split, returns 0if no pipe found.

int parsePipe(char* str, char** strpiped){

    char* token;

    for (int i = 0; i <2 ; ++i) {
       token = (i==0) ? strtok(str, "|") : strtok(NULL, "|");
        if (token == NULL){
            strpiped[i] = NULL;
            break;
        }else{
            strpiped[i] = token;
        }
    }
    return strpiped[1]!=NULL;
}

//the function splites a given string into
// individuals words or tokens based on spaces
// and stores these tokens into arrays.

void parseSpace(char* str, char** parsed){
    int i =0;
    char* token = strtok(str, " ");
    while(token !=NULL && i<MAXLIST){
        parsed[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    //handle case where there are empty strings between
    //spaces.
    for(int j =0; j<i;j++){
        if(strlen(parsed[j])==0){
            for (int k = j; k <i-1 ; ++k) {
                parsed[k] = parsed[k+1];
            }
            parsed[i-1] =NULL;
            i--;
            j--;
        }
    }
    parsed[i]=NULL;
}





int processString(char* str, char** parsed, char** parsedpipe){
    char* strpiped[2];
    int piped = parsePipe(str, strpiped);
    if(piped){
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
    }else{
        parseSpace(str, parsed);
    }
    //checkekk;
    if (ownCmdHandler(parsed)){
        return 0;
    }
    else{
        return 1+piped;
    }
}



int main() {
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsedArgsPiped[MAXLIST];
    int execFlag = 0;
    init_shell();

    while(1){
        //print shell line
        printDir();
        // take input
        takeInput(inputString);

        execFlag = processString(inputString, parsedArgs, parsedArgsPiped);

        if (execFlag==1){
            execArgs(parsedArgs);

        }
        if(execFlag==2){
            execArgsPiped(parsedArgs, parsedArgsPiped);
        }

    }
    return 0;
}
