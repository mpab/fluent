//
// Created by mick on 12.10.17.
//

#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include "console.h"
#include "context.h"

#include "logger.h"

using namespace std;

int __line__ = 1;
char *_src_filename = nullptr;

#ifndef DEBUG_BUILD
bool disable_logging = true;
#else
bool disable_logging = false;
#endif

std::stringstream nullstream;

#ifdef _MSC_VER

#include <windows.h>

struct WinConsole {
    const HANDLE hout;
    WORD orig_col;

    WinConsole()
    : hout(GetStdHandle(STD_OUTPUT_HANDLE)) {

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hout, &csbi);
        orig_col = csbi.wAttributes;

        //cout << "saved orig_col " << orig_col << endl;

        //for (auto i = 0; i != 256; ++i) {
        //    SetConsoleTextAttribute(hout, i);
        //    cout << "i = " << i << ", col = " << i << endl;
        //    reset_col();
        //}
    }

    ~WinConsole() {
        reset_col();
    }
    
    void set_fg_col(int c) {
        c &= 0x0F;
        c += orig_col & 0xF0;
        SetConsoleTextAttribute(hout, c);
    }

    void reset_col() {
        SetConsoleTextAttribute(hout, orig_col);
        //cout << "restored orig_col " << orig_col << endl;
    }
};

WinConsole win_console;

const char* win_console_set_fg_col(int c) { win_console.set_fg_col(c); return ""; }
const char* win_console_reset_col() { win_console.reset_col(); return ""; }

#endif

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
    Stream info_stream(std::cout, ANSI_COLOR_RESET);

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

    void info(const char* s) {
        if (!s) return;
        cout << s << endl;
    }

    void msg(const char* s, const char *c, bool inspect_and_exit) {
        if (!s) return;
        
        cout << c << s;

        if (!console::repl()) {
            cout << ", in file: " << _src_filename << ", at line " << __line__ << ANSI_COLOR_RESET << endl;
        }

        cout << ANSI_COLOR_RESET << endl;

        if (inspect_and_exit) {
            context::inspect();
            exit(-1);
        }
    }

    void warn(const char *s) {
        if (console::repl()) {
            msg(s, ANSI_COLOR_YELLOW, false);
        } else {
            // elevate to error
            error(s);
        }
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
