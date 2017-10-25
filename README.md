# fluent

...a simple, intuitive programming language (and it's cross-platform).

### Variables:

Integer, floating point and string variables can be used.
Valid names start with a letter or underscore '_'.

### Arithmetic operations:

'+' '/' '-' '+' '^' operations are supported with numeric types.
Some operations are also valid with string types.

### Logic operations:

'>' '>=' '<' '<=' '==' '!=' operations are supported with numeric types.
Some operations are also valid with string types.

### Samples:

See the samples folder.

### Build:

On OSX and Linux: use the ./build script.

On Windows: tested using VS2015, use build.bat or load the solution in the vs2015 folder.

- For a command line build (build.bat) make sure VS tools are available (hint: use vsvars32).
- Coloured logging is disabled on Windows.

#### Build prerequisites:

C++11, boost, lex, yacc...

- General
  - yacc (bison) should be > 3.x
- Windows
  - boost: the VS project expects to find boost in C:\Apps\boost_%version% - fix as required.
  - lex, yacc For Windows use this port: https://sourceforge.net/projects/winflexbison/ and **make sure the folder where you unzip this is on your path**.
- OSX and Linux
  - google is your friend (for the purposes of search...)
  - vscode uses an older, incompatible version of yacc, use homebrew to get the latest, and update your path settings accordingly.
- Build weirdness: OSX and Linux
  - The build script copies the output executables to ~/bin/fluent and ~/bin/dfluent - comment this out in the build script or add the folder as required.
- Build weirdness: Windows
  - The build script copies the output executables to C:\Apps\bin - comment this out in the build script or add the folder as required.
