#pragma once
#include <cctype>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef enum REDIRECTION {PIPE, OUTPUT, INPUT} REDIRECTION;

std::string getwhoami();
std::string get_directory();
std::vector<std::string> split(std::string line);
bool process(std::vector<std::string>& tokens);
char** convert_for_exec(std::vector<std::string>& args);
void delete_clist(char** cargs, size_t sz);
bool pipe_process(std::vector<std::vector<std::string>> pipeargs);
bool redirect(std::vector<std::vector<std::string>> pipeargs, REDIRECTION rd);
