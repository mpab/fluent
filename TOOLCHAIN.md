# Toolchain

C++11, boost, lex, yacc...

## TODO

fix the cmake project build to regenerate code from within Visual Studio

## General
  - yacc (bison) should be > 3.x

## Windows
- boost: <https://www.boost.org/users/download/>
  - BOOST_HOME=%APPS%\boost_1_72_0
- (f)lex, yacc (bison): <https://sourceforge.net/projects/winflexbison/>
  - FLEX_BISON_HOME=%APPS%\win_flex_bison-2.5.18
  - PATH=%PATH%;%FLEX_BISON_HOME%
- CMake: <https://cmake.org/download/>
  - CMAKE_HOME=%APPS%\cmake-3.17.1-win64-x64
  - PATH=%PATH%;%CMAKE_HOME%\bin


## OS X
Google is your friend (for the purposes of search...)
OS X uses an older, incompatible version of yacc, use homebrew to get the latest, and update your path settings accordingly.

- brew install bison
- brew install flex

.zshrc

```zsh
export LDFLAGS="-L/usr/local/opt/flex/lib"
export CPPFLAGS="-I/usr/local/opt/flex/include"
export PATH="/usr/local/opt/flex/bin:$PATH"

export LDFLAGS="-L/usr/local/opt/bison/lib"
export PATH="/usr/local/opt/bison/bin:$PATH"

export CPATH=/usr/local/opt/flex/include/
```

The last line is important, else the old headers are used