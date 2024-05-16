# Project Tracking

## Done

2024.05
fix cross-platform build date; fix reduce conflict in grammar; squish msvc warnings; improve unistd.h fix
now supports Windows, Mingw64 and Ubuntu builds
hacked-in shebang support  
fixed quoted string parsing regression  
added modulo operator (%) - can now implement fizzbuzz  
Dusted off and cleaned-up

2017.10  
FIXED:  
parser.y  
arithmetic: evaluation of {symbol} - NUMERIC: syntax error, unexpected '-', expecting TOK_ENDL
works in brackets
works with assignment - e.g. n=n-1, r=r-1.0

2017.10  
Node - use visitor pattern for accessing value

2017.10  
refactor node to use boost:variant

2017.10  
basic variable handling (integer, real, string, sequence)

## Doing

editor syntax highlighting (textmate, vscode)  
- example at <https://github.com/Draco-lang/Compiler/tree/main/src>

## Todo

Fix the windows build

Test on macOS

## Notes/Wishlist

functions/objects  
- define function syntax & implement in parser.y
- implement function model in node/context
- define object syntax & implement in parser.y
- implement object model in node/context
- closures?

loop implementation using sequence  
- statement lists
- program counter

graphics output  
- 2d plotting
- lines
- text
- graphics buffer

file i/o
- opening files
- displaying graphics

syntax error messages
- collate messages, remove duplicates, show at end of parse
- inspect error in yyerror and substitute better text for types - e.g. 'T_STRING'
