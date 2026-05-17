#include "shell.hpp"
#include <cstdlib>

std::string get_directory() {
    char* direc = get_current_dir_name();
    std::string direcs = std::string(direc);
    std::string tilde = "~";
    std::string test_against = "/home/"+getwhoami(); 
    if (direcs.find(test_against) != std::string::npos) {
        direcs.replace(0, test_against.size(), tilde);
    }
    return direcs;
}

std::string getwhoami() {
    const char* user = getlogin();
    if (user == nullptr) {
        user = getenv("USER");
    }
    return user == nullptr ? "unknown" : user;
}

int main() {
    while (true) {
        std::string line;
        std::cout << getwhoami() << ":" << get_directory() << "$ ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        std::vector<std::string> tokens = split(line);
        process(tokens);
        
    }
}
