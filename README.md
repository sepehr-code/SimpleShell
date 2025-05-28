# SimpleShell

A simplified Unix shell implementation in C that demonstrates key system programming concepts including process management, system calls, and I/O redirection.

## Features

### Core Functionality
- **Command Execution**: Run any Linux command available in PATH
- **Built-in Commands**: `cd`, `pwd`, `exit`, `help`
- **Process Management**: Proper forking and process waiting
- **Signal Handling**: Graceful handling of SIGINT and SIGQUIT
- **Memory Management**: Proper allocation and cleanup

### Advanced Features
- **I/O Redirection**: 
  - Output redirection (`>`)
  - Append redirection (`>>`)
  - Input redirection (`<`)
- **Pipes**: Chain commands with `|`
- **Colorized Output**: Enhanced user experience with colors
- **Dynamic Prompt**: Shows current user and working directory

## Learning Outcomes

This project demonstrates:
- **System Calls**: `fork()`, `execvp()`, `wait()`, `pipe()`, `dup2()`
- **Process Management**: Creating and managing child processes
- **String Parsing**: Tokenizing user input
- **File Descriptors**: I/O redirection and pipes
- **Signal Handling**: Responding to user interrupts
- **Memory Management**: Dynamic allocation and proper cleanup

## Project Structure

```
SimpleShell/
├── main.c          # Entry point and main shell loop
├── shell.h         # Header file with declarations and constants
├── parser.c        # Input parsing and tokenization
├── executor.c      # Command execution and process management
├── builtins.c      # Built-in command implementations
├── Makefile        # Build configuration
└── README.md       # This file
```

## Building and Running

### Prerequisites
- GCC compiler
- Linux/Unix environment
- Make utility

### Build Commands

```bash
# Build the shell
make

# Build and run immediately
make run

# Build with debug information
make debug

# Build optimized release version
make release

# Clean build artifacts
make clean

# Show all available targets
make help
```

### Running the Shell

```bash
# Run the compiled shell
./simpleshell
```

## Usage Examples

### Basic Commands
```bash
SimpleShell> ls -la
SimpleShell> pwd
SimpleShell> cd /home/user
SimpleShell> mkdir test_directory
```

### Built-in Commands
```bash
SimpleShell> help          # Show help information
SimpleShell> pwd           # Print working directory
SimpleShell> cd            # Change to home directory
SimpleShell> cd /tmp       # Change to specific directory
SimpleShell> exit          # Exit the shell
SimpleShell> exit 0        # Exit with specific code
```

### I/O Redirection
```bash
SimpleShell> ls > files.txt           # Redirect output to file
SimpleShell> echo "Hello" >> log.txt  # Append to file
SimpleShell> cat < input.txt          # Read from file
```

### Pipes
```bash
SimpleShell> ls -la | grep txt         # Filter ls output
SimpleShell> cat file.txt | wc -l     # Count lines in file
SimpleShell> ps aux | grep shell      # Find shell processes
```

## Key Implementation Details

### Process Management
- Uses `fork()` to create child processes
- Parent process waits for child completion with `waitpid()`
- Proper error handling for failed system calls

### Memory Management
- Dynamic allocation for command tokens
- Proper cleanup to prevent memory leaks
- Safe string handling with bounds checking

### I/O Redirection
- Uses `dup2()` to redirect file descriptors
- Supports input (`<`), output (`>`), and append (`>>`) redirection
- Proper file descriptor management

### Pipes
- Creates pipe with `pipe()` system call
- Forks two processes for pipe endpoints
- Proper file descriptor closing and process synchronization

## Error Handling

The shell includes comprehensive error handling for:
- Memory allocation failures
- Invalid commands
- File operation errors
- Process creation failures
- Signal handling

## Development and Testing

### Memory Leak Detection
```bash
# Check for memory leaks with valgrind
make memcheck
```

### Code Formatting
```bash
# Format code (requires clang-format)
make format
```

### Installation
```bash
# Install to system PATH
make install

# Remove from system
make uninstall
```

## Signal Handling

- **Ctrl+C (SIGINT)**: Displays new prompt without exiting
- **Ctrl+\\ (SIGQUIT)**: Shows message about using 'exit' command
- **Ctrl+D (EOF)**: Gracefully exits the shell

## Limitations

- Single pipe support (no chaining multiple pipes)
- No background process support (`&`)
- No command history
- No tab completion
- No environment variable expansion beyond basic PATH lookup

## Future Enhancements

Potential improvements could include:
- Multiple pipe chaining
- Background process execution
- Command history and editing
- Tab completion
- Environment variable expansion
- Job control
- Configuration file support

## Contributing

This is an educational project demonstrating system programming concepts. Feel free to extend it with additional features or improvements.

## License

This project is provided for educational purposes. Feel free to use and modify as needed for learning system programming concepts.
