#ifndef __CMDCOMMAND_HPP__
#define __CMDCOMMAND_HPP__ 1

#include "lexeme.hpp"
#include <list>
#include <string>

namespace CMD {
namespace Command {
using namespace std;

string evaluate(list<CMD::lexeme> &lexemes);

} /* end namespace Command */
} /* end namespace CMD */
#endif /* END __CMDCOMMAND_HPP__ */
