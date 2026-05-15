#include "shell.hpp"
#include <cstdlib>

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
        std::cout << getwhoami() << ":$ ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        std::vector<std::string> tokens = split(line);
        process(tokens);
        
    }
}
