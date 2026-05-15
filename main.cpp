#include "shell.hpp"

int main() {
    while (true) {
        std::string line;
        char c;
        std::cout << "&ash: ";
        while (std::cin.get(c)) {
            if (c == '\n')  {
                break;
            }
            line += c;
        }
        std::vector<std::string> tokens = split(line);
        process(tokens);
        
    }
}