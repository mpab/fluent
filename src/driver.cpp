#include <cassert>
#include <cctype>
#include <fstream>

#include "command.hpp"
#include "driver.hpp"

using namespace std;
using T = CMD::Parser::token_type;

CMD::Driver::Driver(Mode mode)
    : mode(mode)
{
    if (mode == Mode::REPL) {
        prompt();
    }
}

CMD::Driver::~Driver()
{
    delete (scanner);
    scanner = nullptr;
    delete (parser);
    parser = nullptr;
}

void CMD::Driver::parse(const char* const filename)
{
    assert(filename != nullptr);
    std::ifstream in_file(filename);
    if (!in_file.good()) {
        exit(EXIT_FAILURE);
    }
    parse_helper(in_file);
    return;
}

void CMD::Driver::parse(std::istream& stream)
{
    if (!stream.good() && stream.eof()) {
        return;
    }
    // else
    parse_helper(stream);
    return;
}

void CMD::Driver::parse_helper(std::istream& stream)
{
    delete (scanner);
    try {
        scanner = new CMD::Scanner(&stream);
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!"
                  << endl;
        exit(EXIT_FAILURE);
    }

    delete (parser);
    try {
        parser = new CMD::Parser((*scanner) /* scanner */, (*this) /* driver */);
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!"
                  << endl;
        exit(EXIT_FAILURE);
    }

    const int accept(0);
    if (parser->parse() != accept) {
        std::cerr << "Parse failed!" << endl;
    }
    return;
}

void CMD::Driver::text(const std::string& text)
{
    auto l = lexeme(text);
    lexemes.push_back(lexeme(text));
}

void CMD::Driver::token(T token)
{
    lexemes.push_back(lexeme(token));
    // cout << "token" << endl;
}

void CMD::Driver::chr()
{
    //cout << "chr(" << c << ")" << endl;
}

void CMD::Driver::space()
{
    // cout << "SPC" << endl;
}

void CMD::Driver::abort()
{
    if (mode == Mode::REPL) {
        exit(EXIT_SUCCESS);
    }
    std::cerr << "abort() outside of REPL exiting!" << endl;
    exit(EXIT_FAILURE);
}

void CMD::Driver::newline()
{
    if (mode == Mode::REPL) {
        if (lexemes.size()) {
            cout << evaluate() << endl;
            lexemes.clear();
        }
        prompt();
    }
    //
    cout << "NL" << endl;
}

void CMD::Driver::prompt() { cout << "=> "; }

string CMD::Driver::evaluate() { return Command::evaluate(lexemes); }
