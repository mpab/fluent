# Fluent

...a simple, intuitive programming language (and it's cross-platform).  
dusted off after a brief hiatus and now uses cmake for the build process

supports shebang #!/... execution of fluent scripts on *nix

## Run

```sh
./interpreter/--build
./interpreter/--run {source file}
```

## Install

looks for $HOME/bin and copies the executable there  
(if it's not on your path, it won't work...)

```sh
./interpreter/--build
./interpreter/--install {source file}
```

## Examples

See the examples folder.  
**NOTE** not all scripts will work due to unimplemented features

## Build

Use the script

```sh
interpreter/--build
```

- Build notes - the output is in ./build/interpreter

## Editor syntax-highlighting support

```sh
./scripts/extension-install syntax-fluent
```

## Scripts - more or less self-explanatory

```sh
./interpreter/--*
# and everything in
./scripts
# note that ./scripts/lint uses the alias 'activate' to create a virtual environment
# this 'activate' utility is not included with this project  
```

## Linux

```sh
sudo apt install flex
sudo apt install bison
sudo apt-get install libboost-all-dev
```

## Windows

install the pre-requisites as described below

```dos
interpreter\--clean.bat
interpreter\--build.bat
interpreter\--run.bat
```

## Build prerequisites

C++20, cmake, boost, flex, bison...

- General
  - yacc (bison) should be > 3.x
  - #define FLEX_INCLUDE_DIR and BOOST_HOME
- Windows
  - boost: <https://www.boost.org/users/download/>
  - The project expects to find boost in %BOOST_HOME% e.g. BOOST_HOME=%APPS%\boost_1_72_0 
  - lex, yacc For Windows use this port: <https://github.com/lexxmark/winflexbison/releases>
  - define WIN_FLEX_BISON_HOME as the installation folder in build.bat.
  - add %WIN_FLEX_BISON_HOME% to path
  - cmake - install via winget
  - build chain - install via winget

```dos
winget install Microsoft.VisualStudio.2022.Community --silent --override "--wait --quiet --add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended"

winget install -e --id Kitware.CMake
```

- Windows/Mingw64
  - yacc (bison): pacman -S bison
    - change yacc wrapper to call bison if not automatically done
  - lex (flex): pacman -S flex
  - Mingw64 installs the headers to /usr/include
    - so define FLEX_INCLUDE_DIR to point to them

- OSX and Linux
  - google is your friend (for the purposes of search...)
  - xcode uses an older, incompatible version of yacc, use homebrew to get the latest, and update your path settings accordingly.

```sh
brew install boost
brew install flex
brew install bison
```

```sh
# $HOME/.zshrc
# flex and bison
export PATH="$(brew --prefix flex)/bin:$(brew --prefix bison)/bin:$PATH"
# boost & flex for dev
export BOOST_HOME="$(brew --prefix boost)/include"
export FLEX_INCLUDE_DIR="$(brew --prefix flex)/include"
```

## Language Notes

### Variables

Integer, floating point and string variables can be used.
Valid names start with a letter or an underscore '_'.

### Arithmetic operations

'*' '/' '-' '+' '^' '%' operations are supported with numeric types.
Some operations are also valid with string types.

### Logic operations

'>' '>=' '<' '<=' '==' '!=' operations are supported with numeric types.
Some operations are also valid with string types.

---

## Caution

This project is more of a playground than a real language, use under advisement
