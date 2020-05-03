#include <cassert>
#include <iostream>

#include "scanner.hpp"

using namespace std;

int parse_file(std::string filename)
{
    auto scanner = lexer::Scanner(filename);
    scanner.TokenizeUntilEof();
    cout << endl;
    return EXIT_SUCCESS;
}

int parse_console()
{
    auto scanner = lexer::Scanner();
    scanner.TokenizeUntilEof();
    cout << endl;
    return EXIT_SUCCESS;
}

int main(const int argc, const char** argv)
{
    if (argc > 1) {
        assert(argv[1] != nullptr);
        return parse_file(argv[1]);
    }

    return parse_console();
}
