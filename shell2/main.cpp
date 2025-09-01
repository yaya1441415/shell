#include <iostream>
#include <windows.h>
#include <direct.h>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>

#define MAXLIST 100
#define MAXCOM 1000
#define clear() system("cls")  // Use Windows clear

bool shouldExit = false;

void init_shell();
int takeInput(char* str);
void printDir();
void execArgs(char** parsed);
void execArgsPiped(char** parsed, char** parsedpipe);
void openHelp();
int ownCmdHandler(char** parsed);
int parsePipe(char* str, char** strpiped);
void trimWhitespace(char* str);
void parseSpace(char* str, char** parsed);
int processString(char* str, char** parsed, char** parsedpipe);

void init_shell() {
    clear();
    std::cout << "\n**** MY SHELL ****\n- USE AT YOUR OWN RISK -\n";
    char* username = getenv("USERNAME");
    if (username) std::cout << "\nUSER: @" << username << "\n";
    else std::cout << "\nUSER: Unknown\n";
    Sleep(1500);
    clear();
}

int takeInput(char* str) {
    std::string buffer;
    std::cout << "\n>>> ";
    if (!std::getline(std::cin, buffer)) {
        shouldExit = true;
        return 1;
    }
    if (!buffer.empty()) {
        strcpy(str, buffer.c_str());
        return 0;
    }
    return 1;
}

void printDir() {
    char buffer[MAX_PATH];
    if (GetCurrentDirectoryA(MAX_PATH, buffer)) {
        std::cout << "\n[" << buffer << "]$ ";
    } else {
        std::cout << "\n[Unknown Directory]$ ";
    }
}

void execArgs(char** parsed) {
    std::string command = "cmd /c ";
    for (int i = 0; parsed[i]; i++) {
        command += parsed[i];
        command += " ";
    }

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(NULL, const_cast<char*>(command.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "Failed to execute command. Error: " << GetLastError() << std::endl;
        return;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void execArgsPiped(char** parsed, char** parsedpipe) {
    HANDLE pipeRead, pipeWrite;
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
    if (!CreatePipe(&pipeRead, &pipeWrite, &sa, 0)) {
        std::cerr << "Pipe creation failed: " << GetLastError() << std::endl;
        return;
    }

    STARTUPINFOA si1 = { sizeof(si1) };
    STARTUPINFOA si2 = { sizeof(si2) };
    PROCESS_INFORMATION pi1, pi2;
    ZeroMemory(&pi1, sizeof(pi1));
    ZeroMemory(&pi2, sizeof(pi2));

    si1.dwFlags = STARTF_USESTDHANDLES;
    si1.hStdOutput = pipeWrite;
    si1.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si1.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    si2.dwFlags = STARTF_USESTDHANDLES;
    si2.hStdInput = pipeRead;
    si2.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si2.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    std::string cmd1 = "cmd /c ";
    for (int i = 0; parsed[i]; i++) { cmd1 += parsed[i]; cmd1 += " "; }
    std::string cmd2 = "cmd /c ";
    for (int i = 0; parsedpipe[i]; i++) { cmd2 += parsedpipe[i]; cmd2 += " "; }

    if (CreateProcessA(NULL, const_cast<char*>(cmd1.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si1, &pi1)) {
        if (CreateProcessA(NULL, const_cast<char*>(cmd2.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2)) {
            CloseHandle(pipeWrite);
            CloseHandle(pipeRead);
            WaitForSingleObject(pi1.hProcess, INFINITE);
            WaitForSingleObject(pi2.hProcess, INFINITE);
            CloseHandle(pi2.hProcess); CloseHandle(pi2.hThread);
        }
        CloseHandle(pi1.hProcess); CloseHandle(pi1.hThread);
    }
    CloseHandle(pipeRead); CloseHandle(pipeWrite);
}

void openHelp() {
    std::cout << "\n*** WELCOME TO MY SHELL HELP ***\n"
        "Built-in Commands:\n"
        "  exit      - Exit the shell\n"
        "  cd [dir]  - Change directory\n"
        "  help      - Show this help\n"
        "  hello     - Greet the user\n"
        "  mkdir [dir] - Create directory\n"
        "  rmdir [dir] - Remove directory\n"
        "  cls       - Clear screen\n"
        "Features:\n"
        "  Pipe support: command1 | command2\n"
        "  All Windows commands supported\n";
}

int ownCmdHandler(char** parsed) {
    if (!parsed[0]) return 0;
    for (char* p = parsed[0]; *p; ++p) *p = tolower(*p);

    if (strcmp(parsed[0], "exit") == 0) { shouldExit = true; return 1; }
    if (strcmp(parsed[0], "cd") == 0) {
        if (parsed[1]) { if (_chdir(parsed[1]) != 0) std::cerr << "cd: " << parsed[1] << ": No such directory\n"; }
        else _chdir(getenv("USERPROFILE"));
        return 1;
    }
    if (strcmp(parsed[0], "help") == 0) { openHelp(); return 1; }
    if (strcmp(parsed[0], "hello") == 0) { std::cout << "Hello " << getenv("USERNAME") << "!\n"; return 1; }
    if (strcmp(parsed[0], "mkdir") == 0) { if (parsed[1]) _mkdir(parsed[1]); else std::cerr << "mkdir: missing operand\n"; return 1; }
    if (strcmp(parsed[0], "rmdir") == 0) { if (parsed[1]) _rmdir(parsed[1]); else std::cerr << "rmdir: missing operand\n"; return 1; }
    if (strcmp(parsed[0], "cls") == 0) { system("cls"); return 1; }
    return 0;
}

int parsePipe(char* str, char** strpiped) {
    char* pipePos = strchr(str, '|');
    if (!pipePos) { strpiped[0] = str; strpiped[1] = NULL; return 0; }
    *pipePos = '\0';
    strpiped[0] = str; strpiped[1] = pipePos + 1;
    trimWhitespace(strpiped[0]); trimWhitespace(strpiped[1]);
    return 1;
}

void trimWhitespace(char* str) {
    if (!str) return;
    char* start = str;
    char* end = str + strlen(str) - 1;
    while (isspace((unsigned char)*start)) start++;
    while (end > start && isspace((unsigned char)*end)) end--;
    memmove(str, start, end - start + 1);
    str[end - start + 1] = '\0';
}

void parseSpace(char* str, char** parsed) {
    int i = 0;
    char* token = strtok(str, " ");
    while (token && i < MAXLIST - 1) { parsed[i++] = token; token = strtok(NULL, " "); }
    parsed[i] = NULL;
}

int processString(char* str, char** parsed, char** parsedpipe) {
    char* strpiped[2];
    int piped = parsePipe(str, strpiped);
    if (piped) { parseSpace(strpiped[0], parsed); parseSpace(strpiped[1], parsedpipe); }
    else parseSpace(str, parsed);
    return ownCmdHandler(parsed) ? 0 : 1 + piped;
}

int main() {
    char inputString[MAXCOM], *parsedArgs[MAXLIST] = {0}, *parsedArgsPiped[MAXLIST] = {0};
    init_shell();
    while (!shouldExit) {
        printDir();
        memset(parsedArgs, 0, sizeof(parsedArgs));
        memset(parsedArgsPiped, 0, sizeof(parsedArgsPiped));
        if (takeInput(inputString) != 0) continue;
        int execFlag = processString(inputString, parsedArgs, parsedArgsPiped);
        if (execFlag == 0) continue;            // built-in handled
        if (execFlag == 1) execArgs(parsedArgs); // external command
        else if (execFlag == 2) execArgsPiped(parsedArgs, parsedArgsPiped); // piped
    }
    return 0;
}
