# ELYES Employee and Student Management System Library

Copyright (c) 2024-2025 ELYES. All rights reserved.

This is a C library that provides functionality for managing employee and student records. The library is designed to be used with Flutter applications through FFI (Foreign Function Interface).

Created by ELYES

## Version Information

Current Version: 1.0.0

## Features

- Student Management: - Done by ELYES
  - Add, delete, and modify student records
  - Search students by name or roll number
  - Retrieve student information by index or roll number
  - Get total student count
  - Input validation for all fields
  - Secure string handling

- Employee Management: - Done by ELYES
  - Add, delete, and modify employee records
  - Search employees by name or ID
  - Retrieve employee information by index or ID
  - Get total employee count
  - Input validation for all fields
  - Secure string handling

## Building the Library

### Prerequisites - Done by ELYES

- GCC compiler (version 4.8 or higher)
- Make build system
- Standard C libraries
- Root privileges (for installation)
- lcov (for coverage reports)

### Build Configurations - Done by ELYES

The library can be built in different configurations:

1. Debug Build:
```bash
make debug
```
- Includes debug symbols
- No optimization
- Debug logging enabled

2. Release Build:
```bash
make release
```
- Optimized for performance
- Debug logging disabled
- Stripped debug symbols

3. Profile Build:
```bash
make profile
```
- Includes profiling information
- Moderate optimization
- Debug symbols included

### Build Instructions - Done by ELYES

1. Clone the repository
2. Navigate to the project directory
3. Choose a build configuration and run:
```bash
make clean
make <configuration>  # debug, release, or profile
make install  # Requires root privileges
```

### Platform-Specific Instructions - Done by ELYES

#### Linux
```bash
sudo make install
```

#### Windows
1. Install MinGW or MSYS2
2. Add MinGW/bin to PATH
3. Run:
```bash
mingw32-make install
```

### Testing and Coverage - Done by ELYES

To run tests:
```bash
make test
```

To generate coverage report:
```bash
make coverage
```
This will create a coverage report in the `coverage_report` directory.

## API Documentation - Done by ELYES

### Error Handling

All functions return an integer status code:
- 0: Success
- -1: General error
- -2: Invalid input
- -3: File operation error
- -4: Memory allocation error

### Input Validation

The library performs validation on all inputs:
- String lengths (name, email, phone, etc.)
- Numeric ranges (age, GPA, attendance)
- Date formats
- Required fields

### Function Return Values

#### Student Management
- `add_student`: Returns roll number on success, -1 on failure
- `delete_student`: Returns 1 on success, -1 on failure
- `modify_student`: Returns 1 on success, -1 on failure
- `get_student_count`: Returns count of students, -1 on error
- `search_student_by_name`: Returns number of matches found, -1 on error

#### Employee Management
- `add_employee`: Returns employee ID on success, -1 on failure
- `delete_employee`: Returns 1 on success, -1 on failure
- `modify_employee`: Returns 1 on success, -1 on failure
- `get_employee_count`: Returns count of employees, -1 on error
- `search_employee_by_name`: Returns number of matches found, -1 on error

## Usage in Flutter - Done by ELYES

To use this library in a Flutter application:

1. Add the following to your `pubspec.yaml`:

```yaml
dependencies:
  ffi: ^2.0.1
```

2. Create a Dart file to define the FFI interface:

```dart
import 'dart:ffi';
import 'package:ffi/ffi.dart';

typedef InitializeNative = Int32 Function();
typedef Initialize = int Function();

typedef AddStudentNative = Int32 Function(Pointer<Utf8> name, Pointer<Utf8> roll, Int32 age);
typedef AddStudent = int Function(Pointer<Utf8> name, Pointer<Utf8> roll, int age);

// Add other function typedefs as needed

class ManagementLibrary {
  static final DynamicLibrary _lib = DynamicLibrary.open('libelyes_management.so');
  
  static final Initialize initialize = _lib
      .lookupFunction<InitializeNative, Initialize>('initialize');
      
  static final AddStudent addStudent = _lib
      .lookupFunction<AddStudentNative, AddStudent>('add_student');
      
  // Add other function bindings as needed
}
```

3. Use the library functions in your Flutter code:

```dart
void main() {
  // Initialize the library
  final result = ManagementLibrary.initialize();
  if (result != 0) {
    print('Failed to initialize library');
    return;
  }
  
  // Add a student
  final name = 'John Doe'.toNativeUtf8();
  final roll = '12345'.toNativeUtf8();
  final age = 20;
  
  final addResult = ManagementLibrary.addStudent(name, roll, age);
  if (addResult == 0) {
    print('Student added successfully');
  } else {
    print('Failed to add student');
  }
  
  // Free allocated memory
  calloc.free(name);
  calloc.free(roll);
}
```

## Error Handling - Done by ELYES

The library provides error handling through the `get_last_error()` function. When an operation fails, you can call this function to get a description of the error.

## Data Storage - Done by ELYES

The library stores data in two binary files:
- `elyes_students.dat`: Contains student records
- `elyes_employees.dat`: Contains employee records

These files are automatically created in the current working directory when the library is initialized.

## Troubleshooting - Done by ELYES

### Common Issues

1. Library not found
   - Ensure the library is installed in `/usr/local/lib/`
   - Run `ldconfig` to update the library cache
   - Check if the library path is in `LD_LIBRARY_PATH`

2. Permission denied
   - Run installation with sudo
   - Check file permissions on data files
   - Ensure write permissions in the working directory

3. Memory issues
   - Always free allocated memory in Flutter code
   - Use try-finally blocks for memory management
   - Check for memory leaks using valgrind

### Debugging

For debug builds:
1. Compile with debug flags: `make debug`
2. Use GDB for debugging:
```bash
gdb ./your_program
(gdb) break management_lib.c:initialize
(gdb) run
```

### Performance Optimization - Done by ELYES

For production use:
1. Use release build: `make release`
2. Profile the application: `make profile`
3. Analyze profiling data:
```bash
gprof ./your_program gmon.out > profile.txt
```

## License

Copyright (c) 2024-2025 ELYES. All rights reserved.

This project is licensed under the MIT License - see the LICENSE file for details.
