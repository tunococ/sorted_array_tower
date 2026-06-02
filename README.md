# sorted_array_tower

A modern C++ library for efficient sorted array operations using C++20.

## Features

- **Template-based SortedArrayTower** data structure with automatic sorted insertion
- **C++20 Support** - Uses concepts and requires clauses for type safety
- **Dual Compilation Modes** - Supports both traditional headers and C++20 modules
- **Efficient Operations** - Binary search for $O((\log n)^2)$ lookups
- **Comprehensive Testing** - Full test suite using doctest framework
- **Modern Build System** - CMake with Conan dependency management

## Usage

## Development

### Prerequisites

- C++ compiler that supports C++20 (GCC 15 or newer, Clang 18 or newer)
- CMake 4.3.2 or newer
- Conan 2.0 or newer

#### Notes on other dependencies

C++ module support
- GNU Make doesn't support C++ modules or multi-config builds.
  - Ninja version 1.12.0 or newer can support both.
- `clang` needs `clang-scan-deps` command to be available.

Coverage information
- `gcovr` is needed to generate coverage reports.
- GCC needs `gcov`. It must be the same version as `gcc`.
- Clang needs `llvm-cov`. It must be the same version as `clang`.

### Build process

The build process consists of the following steps:
1. Conan install stage.
2. CMake configure stage.
3. CMake build stage.

Before you start building, you should be aware if your *build generator* is a
single-config generator or a multi-config generator, as the build process
differs slightly based on this.

A single-config generator needs to choose between the *debug* mode and the
*release* mode in step 1, while a multi-config generator does so in step 3.

<!-- TODO: Expand on Conan profiles and generators -->

### 1. Conan install stage

If you are using a single-config generator, you should pick whether you are
building in the debug mode or the release mode now by running one of the two
commands:

- debug:
  
  ```bash
  conan install . --build=missing -s build_type=Debug
  ```

- release:

  ```bash
  conan install . --build=missing -s build_type=Release
  ```

If you are using a multi-config generator, you should run both commands above.

#### Predefined profiles in [.pr](.pr)

This repository provides some predefined Conan profiles in the subdirectory
[.pr](.pr). There two 3 types of profiles:

- Base profiles: You should use these to specify which compiler to use.
  - [clang](.pr/clang): Use Clang as the compiler.
  - [gcc](.pr/gcc): Use GCC as the compiler.
- Generator add-ons: Add this profile to an existing profile to choose Ninja
  as the generator.
  - [ninja-debug](.pr/ninja-debug): Use Ninja for a single-config debug build.
    This profile already includes `-s build_type=Debug`.
  - [ninja-release](.pr/ninja-release): Use Ninja for a single-config release
    build. This profile already includes `-s build_type=Release`.
  - [ninja-multi-debug](.pr/ninja-multi-debug): Use Ninja for a multi-config
    build, and prepare dependencies for the debug build. This profile already
    includes `-s build_type=Debug`.
  - [ninja-multi-release](.pr/ninja-multi-release): Use Ninja for a
    multi-config build, and prepare dependencies for the release build. This
    profile already includes `-s build_type=Release`.
- Combined profiles: Shortcuts that are combinations of the two options above.
  - [clang-debug](.pr/clang-debug): Same as `clang` and `ninja-debug`.
  - [clang-release](.pr/clang-release): Same as `clang` and `ninja-release`.
  - [clang-multi-debug](.pr/clang-multi-debug): Same as `clang` and
    `ninja-multi-debug`.
  - [clang-multi-release](.pr/clang-multi-release): Same as `clang` and
    `ninja-multi-release`.
  - [gcc-debug](.pr/gcc-debug): Same as `gcc` and `ninja-debug`.
  - [gcc-release](.pr/gcc-release): Same as `gcc` and `ninja-release`.
  - [gcc-multi-debug](.pr/gcc-multi-debug): Same as `gcc` and
    `ninja-multi-debug`.
  - [gcc-multi-release](.pr/gcc-multi-release): Same as `gcc` and
    `ninja-multi-release`.

All the combined profiles assume that Ninja is the generator because it
supports both C++ modules and multi-config builds.

As an example, you can use

```bash
conan install . --build=missing -pr .pr/clang-multi-debug
conan install . --build=missing -pr .pr/clang-multi-release
```

or

```bash
conan install . --build=missing -pr .pr/clang -pr .pr/ninja-debug
conan install . --build=missing -pr .pr/clang -pr .pr/ninja-release
```

to specify that you want to use Clang as the compiler and Ninja as a generator
for a multi-config build.

#### Using modules vs using headers

By default, this library will be built for a consumer that uses C++ modules.
To use headers instead, append `-o use_modules=False` to the above command.
For example,

```bash
conan install . --build=missing -pr .pr/gcc-debug -o use_modules=False
```

will prepare a build system for a library that use headers, where the compiler
is GCC and the build mode is debug.

### 2. CMake configure stage

If you are using a single-config generator, choose the preset consistent with
the build mode you chose earlier.
If you are using a multi-config generator, use `conan-default`.

- single-config, debug:

  ```bash
  cmake --preset conan-debug
  ```

- single-config, release:

  ```bash
  cmake --preset conan-release
  ```

- multi-config:

  ```bash
  cmake --preset conan-default
  ```

*Note: If you want to generate coverage reports, you must add the option
`-DSAT_ENABLE_COVERAGE=ON` to the command at this stage.
See [below](#generating-code-coverage-reports) for more information.*


### 3. CMake build stage

- debug:

  ```bash
  cmake --build --preset conan-debug
  ```

- release:

  ```bash
  cmake --build --preset conan-release
  ```

If you are using a single-config generator, you can only compile code in the
same mode you configured earlier, i.e., only one of the two commands here will
work.

If you are using a multi-config generator, this is the point you choose which
mode to build your code in. You can run both commands to build for both modes.

### Running tests

After you have successfully compiled the code with `cmake --build`, you can run
tests by issuing a `ctest` command. Use the same preset as what you used when
you compiled your code with `cmake --build`.

- For the debug build:

  ```bash
  ctest --preset conan-debug
  ```

- For the release build:

  ```bash
  ctest --preset conan-release
  ```

### Generating code coverage reports

To generate coverage reports, you need to
- have this project as the top-level project.
- have [`gcovr`](https://gcovr.com/en/stable/installation.html) installed.

Code coverage instrumentation can be generated by adding
`-DSAT_ENABLE_COVERAGE=ON` to the CMake configure command in the
[CMake configure stage](#2-cmake-configure-stage) *when this project is the
top-level project*.

- single-config generator (debug):

  ```bash
  cmake --preset conan-debug -DSAT_ENABLE_COVERAGE=ON
  ```

- single-config generator (release):

  ```bash
  cmake --preset conan-release -DSAT_ENABLE_COVERAGE=ON
  ```

- multi-config generator:

  ```bash
  cmake --preset conan-default -DSAT_ENABLE_COVERAGE=ON
  ```

After configuring, the coverage reports can be built with

```bash
cmake --build --preset conan-debug --target coverage
```

or

```bash
cmake --build --preset conan-release --target coverage
```

depending on the build type.

The HTML report will be in `build/Debug/coverage_report/` or
`build/Release/coverage_report/`, depending on the build type.
Open the `index.html` in a web browser to view it.

Alternatively, you can serve the HTML file with Python's http.server.
For example,

```bash
python3 -m http.server --directory build/Debug/coverage_report 8080
```

will make the coverage report from the debug build available at
`localhost:8080`.

### Troubleshooting

- In order to use Clang with C++ modules, you will need `clang-scan-deps` to
  be available. It is a part of [Extra Clang Tools](
    https://clang.llvm.org/extra/index.html).
- In order to generate coverage information with `gcc`, `gcov` must be
  accessible and have the same version as `gcc`.
- In order to generate coverage information with `clang`, `llvm-cov` must be
  accessible and have the same version as `clang`.