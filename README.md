# Periodic Table of Elements Database Maker

A command-line application that takes a JSON-formatted datastore file from (The National Library of Medicine)[https://pubchem.ncbi.nlm.nih.gov/rest/pug/periodictable/JSON?response_type=display] and produces an optimized SQLite database file with a table containing all of the records from the datastore file.

### Build system
- `make` used to build this project
- `clang++` using `std=c++20`
- `clang` used for SQLite3 compilation
- Run `$ make` to build binary executable
- Run `$ make run` to run executable with appropriate command-line arguments
- Run `$ make clean` to cleanup build directory

### Example usage
```
$ ./ptg input_file_path=<path/to/input/file> output_file_path=<path/to/output/file>
```

### Source of elements data
- https://pubchem.ncbi.nlm.nih.gov/rest/pug/periodictable/JSON?response_type=display

> Copyright © 2024 Tyler R. Suehr
