#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include "console.hpp"
#include "driver.hpp"

using namespace std;

void help()
{
    cout << "no parameters enters the REPL, quit exits" << endl;
    cout << "use -o for pipe to cin" << endl;
    cout << "use -f to read from a file" << endl;
    cout << "use -h to get this menu" << endl;
}

int stream(const int argc, const char** argv)
{
    CMD::Driver driver;
    if (strncmp(argv[1], "-o", 2) == 0) {
        driver.parse(cin);
        cout << driver.evaluate() << endl;
        return EXIT_SUCCESS;
    }

    if (strncmp(argv[1], "-f", 2) == 0) {
        driver.parse(argv[2]);
        cout << driver.evaluate() << endl;
        return EXIT_SUCCESS;
    }

    help();

    return EXIT_FAILURE;
}

int main(const int argc, const char** argv)
{
    if (argc >= 2) {
        return stream(argc, argv);
    }

    console::copyright();

    CMD::Driver driver(CMD::Driver::Mode::REPL);
    driver.parse(cin);
    // REPL mode calls driver.evaluate() after every line feed

    return EXIT_SUCCESS;
}
