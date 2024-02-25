# CS130_PZ_C Project Documentation

## Overview
This project is a C application aimed at managing a list of students. It provides a graphical user interface (GUI) for users to add, edit, and remove student entries, as well as save and load them from files. The application utilizes the WinAPI for GUI development on the Windows platform.

## Features
- Add new students: Allows users to input details such as name, surname, index, year, and average grade for a new student.
- Edit existing students: Enables users to modify the details of existing student entries.
- Remove students: Allows users to delete student entries from the list.
- Save and load functionality: Provides options to save the list of students to a file and load them back into the application.
- User-friendly GUI: Utilizes buttons, textboxes, and list view controls for intuitive interaction.

## Project Structure
The project consists of several header and source files:

### Header Files
- `gui.h`: Contains functions and structures for initializing the graphical user interface.
- `student.h`: Defines the `Student` struct and functions for managing student data.
- `listview.h`: Handles the creation and management of the list view control for displaying student entries.
- `editor.h`: Manages the GUI components for adding, editing, and removing student entries.
- `files.h`: Handles file-related operations such as saving and loading student data.

### Source Files
- `main.c`: Contains the `WinMain` function and entry point for the application.
- `gui.c`: Implements functions for initializing and managing the GUI components.
- `student.c`: Implements functions for managing student data, such as adding, editing, and removing students.
- `listview.c`: Implements functions for initializing and managing the list view control.
- `editor.c`: Implements functions for handling user interaction with the editor GUI.
- `files.c`: Implements functions for file-related operations, such as saving and loading student data.

## Building and Running
To build the project, compile all source files and link them together. Ensure that the necessary WinAPI libraries are included during compilation.

To run the application, execute the generated executable file. Ensure that all required DLL files are present in the same directory as the executable.

## Usage
Upon running the application, the user is presented with a graphical interface for managing student entries. Use the provided buttons to add, edit, and remove students. The list view control displays the current list of students, and file-related buttons allow saving and loading student data.

## Dependencies
This project relies on the WinAPI for GUI development on the Windows platform. Ensure that the necessary Windows libraries and headers are available during compilation.

## Contributors
- Aleksa (aleksa9192@gmail.com)
