# Pattern finder

Pattern Finder is a simple program for finding the offset of the return address in a program vulnerable to buffer overflow.

# Compiling

`make build`

Makefile is very simple, feel free to change it if you need to.

# Parameters

### Pattern create

Using `-f` followed by the size of the string you want, you will create a pattern.

`$ ./pattern-finder -f 40`

### Pattern search

Using `-l` followed by the pattern you find, it will return the exact offset of where the return address is located.

`$ ./pattern-finder -l 2Ab3`


