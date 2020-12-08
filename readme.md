# Advent of Code 2020

As described on the [about page](https://adventofcode.com/about) of "Advent of Code":

>Advent of Code is an Advent calendar of small programming puzzles for a variety of skill sets and skill levels that can be solved in any 
>programming language you like. People use them as a speed contest, interview prep, company training, university coursework, practice problems, or 
>to challenge each other.

This is my repository with solutions to the different puzzles **written in C**.

## Goals

I do not intend to write the fastest, shortest or in any other way "best" solution to the given puzzles, as this is more some kind of a hobby side 
project for my free time for learning C. Nevertheless I set the following goals I hope to be able to achieve while solving these puzzles:

* **Readability and structure:** I do not indend to write the fewest LoC but rather have a well structured project with readable and maintainable 
code suitable for the given problem
* **Efficiency:** Even though these will not be the most optimised solutions to the given puzzles, I try to write the code as efficient as 
possible (for me) without conflicting other goals
* **Portability and Compatibility:** This is probably the most difficult, as I try to solve all puzzles being compatible with older compilers and 
OS independant by coding C99 compliant without any external libraries, also including basic functionality like regex from POSIX `regex.h`
* **Zero dependency** As stated in the previous point, I intend to not use any libraries beside the C standard library (and the meson build system 
for comfortable build, but I do not count that as it is not part of the puzzle solution)

## Project structure

```
├── meson.build         <-- root meson.build file
├── readme.md           <-- you are here
└── src                 <-- source directory
    |
    ├── meson.build     <-- meson.build file referencing the subfolders of src
    |
    ├── dayX            <-- subfolder for each day
    │   ├── input.txt   <-- puzzle input taken from adventofcode.com
    │   ├── meson.build <-- specific meson.build file creating the executable
    │   ├── puzzle.c    <-- c source file containg at least the main() function 
    |   └── (...)       <-- additional other sources
    ├── (...)
    |
    └── util            <-- subfolder containing utils needed in many puzzles
        ├── meson.build <-- meson.build file for creating library from utils
        └── (...)       <-- c source files for utils
```
## Build and execution

This project uses [the Meson Build System](https://mesonbuild.com/index.html),
for further information, tutorials and installation instructions see the [documentation]((https://mesonbuild.com/index.html)).

In order to generate the ninja build files, run
```
meson build
```

After that execute the following command to build the whole project and compile the executables:
```
ninja -C build
```

These steps will create an executable for each day in `build/src/dayX/` called `puzzle` that takes exactly one command line argument, which is the
path to the input file (also available in the same directory). This executable solves both of the puzzles of that day and writes the solutions to
stdout.

Example: In order to execute the puzzles of day 2, run:
```
./build/src/day2/puzzle ./build/src/day2/input.txt
```

You could also run all executables at once by running `meson test` from within the `build` dir which will generate a test log in `build/meson-logs/testlog.txt`. This will also include Valgrind memcheck for finding possible memory leaks (only if valgrind is installed). If you want to omit these tests, run `meson test --suite basic`