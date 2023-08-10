# Student Management System

The Student Management System is an application that offers functionalities/services required in an academic setting involving student activities. This project is developed using C-language on a Linux-based environment and provides features for registering, searching, updating, and deleting student records for a specific academic club.

## Table of Contents

- [Context](#context)
- [Problem Overview](#problem-overview)
- [Project Features](#project-features)
- [Compilation and Execution](#compilation-and-execution)
- [Performance Measurement](#performance-measurement)
- [Assumptions](#assumptions)
- [Known Issues](#known-issues)
- [Future Improvements](#future-improvements)
- [Group Details](#group-details)
- [References](#references)

## Context

In an academic institute, students can participate in various activities organized by different clubs. This project focuses on managing student records for one such club. The application allows administrators, managers, and members to interact with the system through a command-line interface. It uses a file-based database to store records and a main-memory linked list to keep track of recently accessed data efficiently.

## Problem Overview

The Student Management System aims to provide the following services:

1. Registration: Adding student records to the database.
2. Search: Retrieving student records based on student ID.
3. Update: Modifying student records, especially for hostel room updates.
4. Delete: Removing student records from the database.

The system supports three types of users: administrators, managers, and members. The application listens to requests from users or other programs, which may involve the operations mentioned above.

## Project Features

- File-based database for storing student records.
- Main-memory linked list to keep track of recently accessed data.
- Command-line interface for user interaction.
- Support for registration, search, update, and delete operations.
- Differentiate between administrator, manager, and member roles.

## Compilation and Execution

The project can be compiled and executed using the provided `run.sh` script. The script will compile all the source files for each module and create executable files. To run the project, use the following command:

```bash
run.sh  
```

## Performance Measurement

The project includes a performance measurement module to measure throughput and mean response time for different request generation scenarios. The results of these measurements are used to assess the system's performance under varying loads.

### Request Generation

The project provides two request generation scenarios:

1. Request Generator - Regular: This program generates a total of 50 pairs of random numbers (student id=[1-100], op id=[0-3]) at an interval of one second. The first number represents the student id, and the second number represents the operation id. For update operations, the program also chooses a random value for the hostel room as the update candidate.

2. Request Generator - Random: This program generates a total of 20 pairs of random numbers, similar to the first program. However, after generating each pair, this program waits for a random amount of time between one to three seconds before generating the next pair.

### Scenario 1

To measure the throughput and mean response time for both request generators, run the performance measurement module for each scenario and record the results.

### Scenario 2

To measure the impact of increasing the main-memory size by 20% and then 50%, modify the main-memory linked list to accommodate more nodes. Run the performance measurement module for each scenario and compare the results with the original main-memory size.

### Scenario 3

To measure the impact of faster memory technology, assuming both the disk and main-memory perform twice as fast, modify the access times in the performance measurement module accordingly. Run the performance measurement module for both the original memory size and increased memory size, and analyze the performance improvement.

## Assumptions

The project makes the following assumptions:

- Student IDs start from 1 and increment by 1 for each new student.
- The system can handle a maximum of 100 students.

## Known Issues

- No known issues at the moment.

## Future Improvements

- Implement more robust error handling and input validation.
- Add additional user roles and access control features.
- Enhance performance using more efficient data structures and algorithms.

---