
#include "command.hpp"
#include "lexeme.hpp"
#include "parser.hh"
#include <list>
#include <string>

namespace CMD {

namespace Command {

    using namespace std;
    using T = CMD::Parser::token_type;

    string evaluate(list<CMD::lexeme>& lexemes)
    {
        for (auto& it : lexemes) {
            switch (it.TOKEN) {
            case T::END:
                return "end";
                break;

            case T::COMMENT:
                return "comment";
                break;

            case T::IF:
                return "if";
                break;

            case T::ELSE:
                return "else";
                break;

            case T::ELSEIF:
                return "else if";
                break;

            case T::ENDIF:
                return "end if";
                break;

            case T::DO:
                return "do";
                break;

            case T::WHILE:
                return "while";
                break;

            case T::WEND:
                return "wend";
                break;

            default:
                return "%NYI%";
                break;
            }
        }
        return string();
    }

    string as_string(T t)
    {
        string out;

        switch (t) {
        case T::TEXT:
            return "TEXT";
        default:
            return "ERROR: bad symbol";
        }
    }

    string lexeme_info(lexeme& l)
    {
        string out;

        switch (l.type) {
        case lexeme::TOKEN:
            out += as_string(l.token);
            break;

        case lexeme::TEXT:
            out += l.text;
            break;

        default:
            cerr << "Driver::lexeme_info - unknown lexeme type!" << endl;
            exit(EXIT_FAILURE);
            break;
        }

        return out;
    }

    bool equivalent(list<CMD::lexeme>& l, list<T> m)
    {
        if (l.size() == 0) {
            return false;
        }

        if (l.size() != m.size()) {
            return false;
        }

        auto il = l.begin();
        auto im = m.begin();

        while (il != l.end()) {
            if (il->type == lexeme::TOKEN && il->token != *im) {
                // cout << "TOKEN MISMATCH" << endl;
                return false;
            }

            if (il->type == lexeme::TEXT && *im != T::TEXT) {
                // cout << "TEXT MISMATCH" << endl;
                return false;
            }

            ++il;
            ++im;
        }

        return true;
    }

    string get_params(list<CMD::lexeme>& l)
    {
        string out;

        for (auto e : l) {
            if (e.type == lexeme::TEXT) {
                out = out + e.text;
                out = out + " ";
            }
        }

        return out;
    }
} // namespace Command

} // namespace CMD
