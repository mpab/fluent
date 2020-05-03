#include <iostream>
#include "scanner.hpp"
#include <cassert>

using namespace std;

int parse_file(std::string filename)
{
    auto scanner = lexer::Scanner(filename);
    scanner.TokenizeUntilEof();
    cout << endl;
    return EXIT_SUCCESS;
}

int parse_console() {

    auto scanner = lexer::Scanner();
    scanner.TokenizeUntilEof();
    cout << endl;
    return EXIT_SUCCESS;
}

int main(const int argc, const char** argv) {

    if (argc >= 2 && strncmp(argv[1], "-f", 2) == 0) {
        assert(argv[2] != nullptr);
        return parse_file(argv[2]);
    }

    return parse_console();
}
