#pragma once
#include <cctype>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

std::vector<std::string> split(std::string line);
bool process(std::vector<std::string>& tokens);
char** convert_for_exec(std::vector<std::string>& args);
void delete_clist(char** cargs, size_t sz);
bool pipe_process(std::vector<std::vector<std::string>> pipeargs);
