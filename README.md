# RUSH (Rapid Unix SHell)

RUSH is a lightweight Unix-like shell implemented in **C**, providing process management, command parsing, I/O redirection, and parallel execution.

## Features
- **Command Execution:** Uses `fork()` and `execv()` with customizable search paths.
- **Built-ins:**  
  - `cd <dir>` — change working directory  
  - `path <dir1> <dir2> ...` — set executable search paths  
  - `exit` — terminate shell
- **Output Redirection:** Supports `>` for file output using `dup2()` and `open()`.
- **Parallel Execution:** Executes multiple commands concurrently using `&`.
- **Error Handling:** Consistent stderr output for all execution and parsing errors.

## Implementation
- Written in **C99**, using `unistd.h`, `sys/wait.h`, and `fcntl.h`.
- Stack-based argument parsing via `strtok()`.
- Default executable path: `/bin`.
- Synchronous wait for all child processes with `waitpid()`.

## Build & Run
```bash
make
./rush

## File Structure
├── Makefile       # Build instructions
└── rush.c         # Core shell implementation
