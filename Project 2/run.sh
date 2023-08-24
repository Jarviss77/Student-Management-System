#!/bin/bash

# Compile C programs
gcc -o Rec_Gen Record_Generator.c

echo 1st done

gcc -o Server Server.c

echo 2nd done

gcc -o Start_Processor Start_Processor.c

echo 3rd done

# Run the leading Program
./Start_Processor

# Clean up compiled files (optional)
rm Rec_Gen Server Start_Processor

