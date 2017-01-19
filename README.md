<p align="center">
<img src="https://raw.githubusercontent.com/Morphux/Graphic/master/logo/single_penguin.png" /><br />
</p>
<p align="center">
<img src="https://img.shields.io/badge/language-c-blue.svg" /> &nbsp;
<img src="https://img.shields.io/badge/license-Apache--2.0-yellow.svg" /> &nbsp;
<a href="https://travis-ci.org/Morphux/lib"><img src="https://travis-ci.org/Morphux/lib.svg?branch=master"/></a> &nbsp;
<a href="https://scan.coverity.com/projects/lib">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/11517/badge.svg"/>
</a>
<br />
<h1 align="center" style="border:none">Morphux/lib</h1>
<h6 align="center">Morphux C Development Library</h6>
</p>
<p align="center">
<a href="#install">Install</a> • <a href="#test">Test</a> • <a href="#use">Use</a> • <a href="#documentation">Documentation</a>
</p>

# Install
### Clone the repository
```
git clone https://github.com/Morphux/lib.git
```

### Compile
```
make
```

# Test
### Dependencies
```
valgrind
```
### Basic Tests
```
make -C tests check
```
### Memory Tests
```
make -C tests valgrind
```

# Use
Compile with the following options
```
gcc [...] -Llib/ -lmorphux -Ilib/inc 
```
Of course, ```lib``` directory must exist.

In code:
```C
#include <morphux.h>
```

### Documentation
TBD.
