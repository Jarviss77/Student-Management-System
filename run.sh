#!/bin/bash

# Compile C programs
gcc -o a regular_request_generator.c
gcc -o b random_request_generator.c
gcc -o Student-Management-System OS_Project.c

# Run request generator programs in the background
./a &
./b &

# Wait for the background processes to finish
wait

# Run the main program
./Student-Management-System

# Clean up compiled files (optional)
rm a b Student-Management-System 