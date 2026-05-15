#include "shell.hpp"

bool change_directory(std::vector<std::string>& args) {
    if (args.size() < 2) {
        return false;
    }
    if (chdir(args[1].c_str()) == 0) {
        return true;
    }
    return false;
}

bool process(std::vector<std::string>& args) {
    if (args.empty()) {
        return true;
    }
    if (args[0] == "cd") {
        return change_directory(args);
    } 
    char** cargs = new char*[args.size() + 1];

    for (size_t i = 0; i<args.size(); i++) {
        cargs[i] = new char[args[i].size()+1];
        std::strcpy(cargs[i], args[i].c_str());
    }
    cargs[args.size()] = nullptr;
    // run ls

    pid_t pid = fork();
    if (pid < 0) {
        std::cout << "Fork for ls failed";
        return false;
    } else if (pid == 0) {
        // child
        execvp(cargs[0], cargs);
        std::cerr << "Exec failed!" << std::endl;
        _exit(1);
    } else {
        wait(NULL);
    }
    for (size_t i = 0; i<args.size(); i++) {
        delete[] cargs[i];
    }
    delete[] cargs;
    return true;
}

std::vector<std::string> split(std::string line) {
    std::vector<std::string> res;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word) {
        res.push_back(word);
        word="";
    }
    return res;
}
