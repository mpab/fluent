//
// Created by mick on 12.10.17.
//

#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include "console.h"

#include "logger.h"

using namespace std;

int __line__ = 1;
extern char *__file__;

#ifndef DEBUG_BUILD
bool disable_logging = true;
#else
bool disable_logging = false;
#endif

std::stringstream nullstream;

namespace logger {

    void on() {
        disable_logging = false;
        debug("logging=on");
        cout << ANSI_COLOR_RESET;
    }

    void off() {
        debug("logging=off");
        cout << ANSI_COLOR_RESET;
        disable_logging = true;
    }

    class Stream : public std::ostream
    {
        // Write a stream buffer that prefixes each line with Plop
        class StreamBuf : public std::stringbuf
        {
            std::ostream& output;
            const char* text_color;
            public:
                StreamBuf(std::ostream& str, const char* text_color) : output(str), text_color(text_color) {}
    
            // When we sync the stream with the output. 
            // 1) Output color then the buffer
            // 2) Reset the buffer
            // 3) flush the actual output stream we are using.
            virtual int sync () {
                output << text_color << str() << ANSI_COLOR_RESET;
                str("");
                output.flush();
                return 0;
            }
        };
    
        // Stream uses a version of buffer
        StreamBuf buffer;

        public:
            Stream(std::ostream& str, const char* text_color) : std::ostream(&buffer), buffer(str, text_color) {}
    };

    Stream debug_stream(std::cout, ANSI_COLOR_CYAN);
    Stream warn_stream(std::cout, ANSI_COLOR_YELLOW);
    Stream error_stream(std::cout, ANSI_COLOR_RED);

    std::ostream& debug() {
        if (disable_logging) {
            nullstream.clear();
            return nullstream;
        }
        return debug_stream;
    }

    std::ostream& warn() {
        return warn_stream;
    }

    std::ostream& error() {
        return error_stream;
    }

    void debug(const char *s) {
        if (disable_logging || !s) return;
        
        cout << ANSI_COLOR_CYAN << s << ANSI_COLOR_RESET << endl;
    }

    void debug_if(bool b, const char *s) {
        if (disable_logging || !s || !b) return;
        debug(s);
    }

    std::ostream& info() {
        return cout;
    }

    void msg(const char* s, const char *c, bool inspect_and_exit) {
        if (!s) return;
        
        cout << c << s;

        if (__file__) {
            cout << ", in file: " << __file__ << ", at line " << __line__ << ANSI_COLOR_RESET << endl;
            if (inspect_and_exit) {
                console::inspect(true);
                exit(-1);
            }
        }

        cout << ANSI_COLOR_RESET << endl;
    }

    void warn(const char *s) {
        msg(s, ANSI_COLOR_YELLOW, false);
    }

    void warn_if(bool b, const char *s) {
        if (!s || !b) return;
        warn(s);
    }

    void error(const char *s) {
        msg(s, ANSI_COLOR_RED, true);
    }

    void error_if(bool b, const char *s) {
        if (!s || !b) return;
        error(s);
    }
}

void yyerror(const char *s) {
    if (!s) return;

    if (0 == strcmp("syntax error, unexpected $end, expecting T_ENDL", s)) {
        logger::warn("fluent source file must end with a new line");
        return;
    }

    //auto bad_token = context::current_node();

    if (0 == strcmp("syntax error, unexpected T_SYMBOL, expecting '('", s)) {
        logger::error("syntax error, missing '('");
        return;
    }

    if (0 == strcmp("syntax error, unexpected T_SYMBOL", s)) {
        logger::error("syntax error, unexpected symbol");
        return;
    }

    logger::error(s);
}
