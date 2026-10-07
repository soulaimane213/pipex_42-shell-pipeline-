# pipex — Shell Pipeline Rebuilt in C

A from-scratch implementation of the Unix shell pipeline (`< infile cmd1 | cmd2 > outfile`) written in **pure C**.

This project re-creates the behavior of shell pipes, process forking, file descriptor redirection, and dynamic path resolution using low-level POSIX system calls (`pipe`, `fork`, `dup2`, `execve`, `waitpid`).

---

## 📹 Video Walkthrough

Watch the full live development and architecture breakdown on YouTube:

[![Watch on YouTube](https://img.shields.io/badge/YouTube-Watch%20Live%20Stream-red?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=VWbcyg62uqg)

---

## 📂 Project Structure

```
pipex/
├── Makefile       # Multi-file build system with optimization flags
├── README.md
├── pipex.h        # Function prototypes and system headers
├── main.c         # Process forking, pipe redirection, and lifecycle management
└── utils.c        # Custom string splitting (ft_split) and PATH resolution
```

---

## ⚙️ How It Works (Pipeline Architecture)

Running `./pipex infile "cmd1" "cmd2" outfile` behaves identically to `< infile cmd1 | cmd2 > outfile`:

```
                 +-------------------+
  infile ------->|   Child 1 (cmd1)  |
 (STDIN)         +---------+---------+
                           |
                        (STDOUT)
                           |
                     [ PIPE BUFFER ]
                           |
                        (STDIN)
                           |
                 +---------+---------+
                 |   Child 2 (cmd2)  |--------> outfile
                 +-------------------+         (STDOUT)
```

1. **`pipe(p)`**: Creates an unidirectional kernel buffer (`p[0]` = read, `p[1]` = write).
2. **Child 1:** Redirects `infile` to `STDIN` and `p[1]` to `STDOUT`, then executes `cmd1`.
3. **Child 2:** Redirects `p[0]` to `STDIN` and `outfile` to `STDOUT`, then executes `cmd2`.
4. **Concurrent Execution:** Both processes run simultaneously in parallel.
5. **Parent:** Closes both pipe ends and reaps the children using `waitpid()`.

---

## 🛠️ Build & Run

### Compilation

```bash
make
```

### Examples

#### 1. Filter and count lines
```bash
./pipex infile "cat" "wc -l" outfile
# Equivalent to: < infile cat | wc -l > outfile
```

#### 2. Search and sort
```bash
./pipex infile "grep error" "sort -r" outfile
# Equivalent to: < infile grep error | sort -r > outfile
```

---

## 🔬 System Calls Used

| Syscall | Purpose |
| :--- | :--- |
| **`pipe`** | Allocate inter-process communication kernel buffer |
| **`fork`** | Duplicate process for concurrent child execution |
| **`dup2`** | Rebind file descriptors (`stdin` / `stdout`) |
| **`execve`** | Replace child image with target binary |
| **`waitpid`** | Prevent zombie processes and capture exit codes |

---

## 👨‍💻 Author

**Soulaimane FADL**  
YouTube: [@Zero2Segfault](https://www.youtube.com/@Zero2Segfault)
