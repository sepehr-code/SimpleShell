#!/bin/bash

# Test script for SimpleShell
# This script demonstrates all the features of the shell

echo "========================================="
echo "         SimpleShell Test Script"
echo "========================================="
echo

echo "Building the shell..."
make clean && make
echo

echo "Testing basic commands..."
echo -e "pwd\nls\necho 'Basic commands work!'" | ./simpleshell
echo

echo "Testing built-in commands..."
echo -e "help\npwd\ncd /tmp\npwd\ncd\npwd" | ./simpleshell
echo

echo "Testing I/O redirection..."
echo -e "echo 'Testing redirection' > test_output.txt\ncat test_output.txt\necho 'Appending text' >> test_output.txt\ncat test_output.txt\nrm test_output.txt" | ./simpleshell
echo

echo "Testing pipes..."
echo -e "ls -la | grep shell\necho 'pipe test' | cat" | ./simpleshell
echo

echo "Testing error handling..."
echo -e "nonexistentcommand\nls /nonexistent/path" | ./simpleshell
echo

echo "All tests completed!"
echo "You can now run './simpleshell' to use the shell interactively." 