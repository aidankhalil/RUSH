RUSH — Rapid Unix Shell (C)

A lightweight Unix shell written in C that supports running external commands via fork/exec, changing directories, configuring executable search paths, basic output redirection, and parallel command execution.

Build:
make

Run:
./rush

Built-ins:
- exit
- cd <dir>
- path <dir1> <dir2> ...

Notes:
- Default path is /bin
- Supports output redirection using ">"
- Supports parallel commands separated by "&"

Clean:
make clean
