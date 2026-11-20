_This project has been created as part of the 42 curriculum by bucolak and seerel._

# minishell

A small Unix shell written in **C**, modelled on **bash**. It reads a command line, parses it into commands, and executes them with pipes, redirections and environment variables.

## Features

- Interactive prompt with history (GNU **readline**)
- Running programs found in `PATH` or by relative/absolute path
- **Pipes** `|` between any number of commands
- **Redirections** `<` `>` `>>` and **heredoc** `<<`
- **Quotes:** single quotes stop all interpretation, double quotes allow `$` expansion
- **Environment variables** `$VAR` and the last exit status `$?`
- **Signals:** `Ctrl-C` shows a new prompt, `Ctrl-D` exits, `Ctrl-\` does nothing — same as bash
- **Built-ins:** `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

## How It Works

```
input ─▶ lexer / quote handling ─▶ $ expansion ─▶ parser (commands, args, redirections)
      ─▶ heredocs ─▶ pipes + fork / execve, or built-in in the parent ─▶ exit status
```

Each command in a pipeline runs in its own child process. File descriptors are connected with `pipe()` and `dup2()`, and the shell waits for all children and keeps the exit status of the last one, like bash.

## Usage

```bash
make
./minishell
```

```
minishell$ cat << EOF | grep hello > out.txt
> hello world
> EOF
minishell$ echo "user: $USER, status: $?"
```

Requires the `readline` library (`sudo apt install libreadline-dev`).

## Concepts Practised

Processes (`fork`, `execve`, `waitpid`) · file descriptors and `dup2` · pipes · signals · parsing and tokenising · environment management · memory management without leaks

## Authors

- **bucolak** — Buket Çolak
- **seerel**
