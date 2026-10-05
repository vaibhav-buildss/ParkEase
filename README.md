# ParkEase - College Parking Management System

A simple beginner-level C programming project for managing college parking.

## Features

1. Park Vehicle
2. Remove Vehicle
3. Search Vehicle
4. View Currently Parked Vehicles
5. Save currently parked vehicles in a text file

## Parking Slots

- A01 to A20 = Bike slots
- B01 to B10 = Car slots

## Main C Concepts Used

- Variables
- Arrays
- Strings
- if/else
- switch
- for loops
- functions
- file handling
- basic C library functions

No structures, databases, frameworks, or advanced C concepts are required.

## Project Structure

ParkEase/
- backend/
  - main.c
  - api.c
- frontend/
  - index.html
  - style.css
  - app.js
  - server.js
- data/
  - vehicles.txt
- compile.bat
- start.bat
- README.md

## Run the Project

### 1. Compile

Double-click:

compile.bat

or run:

gcc backend\main.c -o backend\main.exe
gcc backend\api.c -o backend\api.exe

### 2. Start website

Double-click:

start.bat

Then open:

http://localhost:3000

## C-only version

If you want to demonstrate only C:

Open terminal in the backend folder and run:

main.exe

## Important

The website is only a simple frontend. The actual parking data is handled by the C program and saved in:

data/vehicles.txt

The project intentionally avoids advanced concepts so that it is easier for beginner C students to understand and explain in a viva.
