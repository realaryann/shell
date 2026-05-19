#include "shell.hpp"
#include <cstdlib>

PLATFORM pt;

std::string get_directory() {
    char direc[PATH_MAX];
    if (getcwd(direc, sizeof(direc)) != NULL) {
        std::string direcs = std::string(direc);
        std::string tilde = "~";
        std::string test_against = "/home/"+getwhoami(); 
        std::string test_against1 = "/Users/"+getwhoami();
        if (direcs.find(test_against) != std::string::npos)  {
            direcs.replace(0, test_against.size(), tilde);
            pt = LINUX;
        } else if (direcs.find(test_against1) != std::string::npos) {
            direcs.replace(0, test_against1.size(), tilde);
            pt = MACOS;
        }
        return direcs;
    }
    return "";
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
        process(tokens, pt);
        
    }
}
