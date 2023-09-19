# File Search and Analysis Tool Readme

This C program, named "search," is a versatile file search and analysis tool designed to help you navigate and analyze directories and files. It offers various features and flags to customize your search and analysis tasks. Below, you'll find information on how to compile, run, and use the tool effectively.

## Usage

### Compilation

To compile the "search" program, you can use the following command:

```bash
gcc -o search search.c
```

Alternatively, you can use the provided Makefile by running:

```bash
make
```

### Basic Usage

To run the "search" program, use the following syntax:

```bash
./search [OPTIONS] [DIRECTORY]
```

- `OPTIONS` are the flags and options you can use to customize your search and analysis.
- `DIRECTORY` is the optional path to the directory you want to search in. If not provided, the current directory is used.

### Options and Flags

The "search" program supports the following options and flags:

- `-S`: Show detailed file information, including permissions, size, and access time.
- `-s SIZE`: Filter files by size. Example: `-s 1024` filters files larger than 1024 bytes.
- `-f PATTERN DEPTH`: Filter files by name pattern and depth. Example: `-f docx 3` filters files with "docx" in their name within a depth of 3 levels.
- `-t [f|d]`: Filter files by type. Use `-t f` to filter regular files and `-t d` to filter directories.
- `-e COMMAND`: Execute a command on each matching file. Example: `-e "cat"` will print the content of matching files.
- `-E COMMAND`: Execute a command after processing all matching files. Example: `-E "echo 'Analysis Complete'"`.

## Example Usage

Here are some example usage scenarios:

1. Search and list all files and directories in the current directory:

   ```bash
   ./search
   ```

2. List files and directories with detailed information:

   ```bash
   ./search -S
   ```

3. Filter files larger than 1024 bytes:

   ```bash
   ./search -s 1024
   ```

4. Filter and list files with "docx" in their name within a depth of 3 levels:

   ```bash
   ./search -f docx 3
   ```

5. List only directories:

   ```bash
   ./search -t d
   ```

6. Execute a command (e.g., `cat`) on each matching file:

   ```bash
   ./search -e "cat"
   ```

7. Execute a command (e.g., `echo`) after processing all matching files:

   ```bash
   ./search -E "echo 'Analysis Complete'"
   ```

## Features

- Recursively traverse directories and list files and directories.
- Filter files by size, name pattern, and type.
- Execute custom commands on files.
- Detailed file information display, including permissions, size, and access time.

## License

This "search" tool is open-source software released under the MIT License. You are free to use, modify, and distribute it as needed. Please see the `LICENSE` file for more details.

## Contributing

If you have suggestions for improvements or find any issues, please feel free to open an issue or create a pull request. Contributions from the community are welcome and appreciated.
