#include "shell.hpp"


bool process(std::vector<std::string>& args) {
    char** cargs = new char*[args.size()];

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
        return false;
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