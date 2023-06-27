# somos
Sega Master System Emulator

## How to compile
This project uses CMake. Create the build files with
```shell
    cmake -S . -B build
```
Then run
```shell
    cmake --build build
```
The program can then be run with 
```shell
    cd build/src && ./somos
```

## How to run tests
This project uses GoogleTest as the testing framework. To run the tests, build and compile the program and then run 
```shell
    cd build/tests && ./somos_tests
```
Or
```shell
    cd build/tests && ctest
```

## How to prepare project for a LSP server
I developed this program while using the LSP server `clangd` which requires a `compile_commands.json` in the project root to work properly. CMake needs to be told to create this file by using the following command to build the project
```shell
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -S . -B build
```
The file can then be added to the project root with a symbolic link
```shell
    ln -s build/compile_commands.json . 
```
