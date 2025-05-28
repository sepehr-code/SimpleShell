# Contributing to SimpleShell

Thank you for your interest in contributing to SimpleShell! This project is designed as an educational tool for learning system programming concepts in C.

## How to Contribute

### Reporting Issues
- Use the GitHub issue tracker to report bugs
- Include detailed steps to reproduce the issue
- Mention your operating system and compiler version

### Suggesting Enhancements
- Open an issue with the "enhancement" label
- Describe the feature and its benefits
- Consider the educational value of the enhancement

### Code Contributions

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature-name`
3. **Make your changes** following the coding standards below
4. **Test your changes**: Run `make clean && make && ./test_shell.sh`
5. **Commit your changes**: Use clear, descriptive commit messages
6. **Push to your fork**: `git push origin feature-name`
7. **Create a Pull Request**

## Coding Standards

- Follow the existing code style
- Use meaningful variable and function names
- Add comments for complex logic
- Ensure proper memory management (no leaks)
- Handle errors gracefully
- Update documentation as needed

## Testing

Before submitting changes:
- Ensure the code compiles without warnings: `make clean && make`
- Run the test script: `./test_shell.sh`
- Test manually with various commands
- Check for memory leaks: `make memcheck` (requires valgrind)

## Educational Focus

Remember that this project is primarily educational. When contributing:
- Prioritize code clarity over optimization
- Add comments explaining system programming concepts
- Consider how changes help demonstrate key concepts
- Maintain the modular structure for learning purposes

## Questions?

Feel free to open an issue for any questions about contributing! 