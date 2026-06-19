# pipex - shell pipeline rebuilt in C

a from scratch implementation of the shell pipe `|` behavior in C , using `fork` , `pipe` , `dup2` , and `execve`

## what it does

takes an input file , two shell commands , and an output file , runs them like this :

```bash
< infile cmd1 | cmd2 > outfile
```

basically the same thing as doing that in your shell , but built manually

## how it works

- opens the input file and creates a pipe
- forks a child process that runs `cmd1` with stdin from the file and stdout into the pipe
- the parent waits , then runs `cmd2` with stdin from the pipe and stdout into the output file
- path resolution is done manually by parsing the `PATH` environment variable

## watch the explanation

i built this live step by step in this video :

[![Watch on YouTube](https://img.shields.io/badge/YouTube-Watch-red?logo=youtube)](https://www.youtube.com/watch?v=VWbcyg62uqg&t=7820s)

## build & run

```bash
gcc main.c utils.c -o pipex
./pipex infile "ls -la" "grep .c" outfile
```

## usage

```
./pipex infile 'cmd1' 'cmd2' outfile
```

## channel

[@Zero2Segfault](https://www.youtube.com/@Zero2Segfault) - low level stuff , assembly , c , systems
