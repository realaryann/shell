#pragma once
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

std::vector<std::string> split(std::string line);
bool process(std::vector<std::string>& tokens);
