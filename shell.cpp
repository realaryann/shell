#include "shell.hpp"

bool change_directory(std::vector<std::string>& args, PLATFORM pt) {
    if (args.size() < 2) {
        return false;
    }
    if (args[1] == "~" && pt == LINUX) {
        args[1] = "/home/"+getwhoami();
    } else if (args[1] == "~" && pt == MACOS) {
        args[1]="/Users/"+getwhoami();
    }
    if (chdir(args[1].c_str()) == 0) {
        return true;
    }
    return false;
}

bool redirect(std::vector<std::vector<std::string>> pipeargs, REDIRECTION rd) {
    char** command = convert_for_exec(pipeargs[0]);
    char** filename = convert_for_exec(pipeargs[1]);
    pid_t p1;
    if (rd == OUTPUT) {
        p1 = fork();
        if (p1 < 0) {
            std::cout<< "Fork failed while redirecting output";
        } else if (p1 == 0) {
            // child
            int fd = open(filename[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                std::cout<< "Failed to open file for output";
                return false;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            if (execvp(command[0], command) < 0) {
                std::cout << "Exec failed while redirecting output";
                exit(1);
            }
        } else {
            wait(NULL);
        }
        delete_clist(command, pipeargs[0].size());
        delete_clist(filename, pipeargs[1].size());
        return true;
    } else if (rd == INPUT) {
        p1 = fork();
        if (p1 < 0) {
            std::cout<< "Fork failed while redirecting input";
        } else if (p1 == 0) {
            // child
            int fd = open(filename[0], O_RDONLY, 0644);
            if (fd < 0) {
                std::cout<< "Failed to open file for input";
                return false;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            if (execvp(command[0], command) < 0) {
                std::cout << "Exec failed while redirecting input";
                exit(1);
            }
        } else {
            wait(NULL);
        }
        delete_clist(command, pipeargs[0].size());
        delete_clist(filename, pipeargs[1].size());
        return true;
    }
    return false;
}

bool pipe_process(std::vector<std::vector<std::string>> pipeargs) {
    int fd[2];
    pid_t p1, p2;

    if (pipe(fd) < 0) {
        std::cout << "Pipe failed";
        return false;
    }

    char** c1_cargs = convert_for_exec(pipeargs[0]);
    char** c2_cargs = convert_for_exec(pipeargs[1]);

    p1 = fork();
    if (p1 < 0) {
        std::cout << "Fork failed while piping";
    } else if (p1 == 0) {
        // child process for LHS
        dup2(fd[1], STDOUT_FILENO);

        close(fd[1]);
        close(fd[0]);

        if (execvp(c1_cargs[0], c1_cargs) < 0) {
            std::cout << "exec failed while piping";
            exit(1);
        }
    }

    p2 = fork();
    if (p2 < 0) {
        std::cout << "Fork failed while piping";
    } else if (p2 == 0) {
        // Child process for RHS
        dup2(fd[0], STDIN_FILENO);

        close(fd[1]);
        close(fd[0]);

        if (execvp(c2_cargs[0], c2_cargs)< 0) {
            std::cout << "exec failed while piping";
            exit(1);
        }
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    delete_clist(c1_cargs, pipeargs[0].size());
    delete_clist(c2_cargs, pipeargs[1].size());
    return true;

}

char** convert_for_exec(std::vector<std::string>& args) {
    char** cargs = new char*[args.size() + 1];

    for (size_t i = 0; i<args.size(); i++) {
        cargs[i] = new char[args[i].size()+1];
        std::strcpy(cargs[i], args[i].c_str());
    }
    cargs[args.size()] = nullptr;
    return cargs;
}

void delete_clist(char** cargs, size_t sz) {
    for (size_t i = 0; i<sz; i++) {
        delete[] cargs[i];
    }
    delete[] cargs;
}

bool process(std::vector<std::vector<std::string>>& args, PLATFORM pt) {
    for(int i = 0; i<args.size(); i++) {
        if (args[i].empty()) {
            continue;
        }
        if (args[i][0] == "cd") {
            change_directory(args[i], pt);
            continue;
        } 

        std::vector<std::vector<std::string>> pipeargs;
        std::vector<std::string> internal;
        REDIRECTION rd;

        for (std::string arg : args[i]) {
            if (arg != "|" && arg != "<" && arg != ">") {
                internal.push_back(arg);
            }
            if (arg == "|") {
                rd = PIPE;
                pipeargs.push_back(internal);
                internal.clear();
            } else if (arg == "<") {
                rd = INPUT;
                pipeargs.push_back(internal);
                internal.clear();
            } else if (arg == ">") {
                rd = OUTPUT;
                pipeargs.push_back(internal);
                internal.clear();
            }
        }
        pipeargs.push_back(internal);

        if (pipeargs.size() > 1) {
            switch(rd) {
                case PIPE:
                    pipe_process(pipeargs);
                    continue;
                case OUTPUT:
                    redirect(pipeargs, OUTPUT);
                    continue;
                case INPUT:
                    redirect(pipeargs, INPUT);
                    continue;
            }
        }

        char** cargs = convert_for_exec(args[i]);
        // run ls

        pid_t pid = fork();
        if (pid < 0) {
            std::cout << "Fork for ls failed";
            continue;
        } else if (pid == 0) {
            // child
            execvp(cargs[0], cargs);
            std::cerr << "Exec failed!" << std::endl;
            exit(1);
        } else {
            wait(NULL);
        }
        
        delete_clist(cargs, args[i].size());
    }
    return true;
}

std::vector<std::vector<std::string>> split_conditionals(std::vector<std::string> tokens) {
    std::vector<std::string> inner;
    std::vector<std::vector<std::string>> res;

    for (std::string str : tokens) {
        if (str == "&&") {
            res.push_back(inner);
            inner.clear();
        } else {
            inner.push_back(str);
        }
    }
    res.push_back(inner);
    return res;
}

std::vector<std::string> split(std::string line) {
    std::vector<std::string> res;
    std::string word;
    char quote = '\0';

    for (char ch : line) {
        if (quote != '\0') {
            if (ch == quote) {
                quote = '\0';
            } else {
                word.push_back(ch);
            }
        } else if (ch == '"' || ch == '\'') {
            quote = ch;
        } else if (std::isspace(static_cast<unsigned char>(ch))) {
            if (!word.empty()) {
                res.push_back(word);
                word.clear();
            }
        } else if (ch == '|') {
            if (!word.empty()) {
                res.push_back(word);
                word.clear();
            }
            res.push_back("|");
        } else {
            word.push_back(ch);
        }
    }

    if (!word.empty()) {
        res.push_back(word);
    }
    return res;
}
