# RUSH (Rapid Unix SHell)

Rush is a Rapid Unix-like Shell written in C. 

## Features
- Run external programs
- Built-in commands: `exit`, `cd`, `path`
- Output redirection with `>`
- Parallel execution with `&`

## Build and Run
```bash
make
./rush
``` 

## Examples
```
rush> ls
rush> cd /tmp
rush> path /bin /usr/bin
rush> ls > out.txt
rush> ls & pwd & whoami
```
