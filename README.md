# sorted_array_tower

A modern C++ library for efficient sorted array operations using C++20.

## Features

- **Template-based SortedArrayTower** data structure with automatic sorted insertion
- **C++20 Support** - Uses concepts and requires clauses for type safety
- **Dual Compilation Modes** - Supports both traditional headers and C++20 modules
- **Efficient Operations** - Binary search for O(log n) lookups
- **Comprehensive Testing** - Full test suite using doctest framework
- **Modern Build System** - CMake with Conan dependency management

## Usage

## Development

### Prerequisites

- C++ compiler that supports C++20 (GCC 14 or newer, Clang 18 or newer)
- CMake 4.3.2 or newer
- Conan 2.0 or newer

### Build process

The build process consists of the following steps:
1. Preparing build environment with Conan.
2. Configuring options with CMake.
3. Compiling code with CMake.

Before you start building, you should be aware if your *build generator* is a
single-config generator or a multi-config generator, as the build process
differs slightly based on this.

A single-config generator needs to choose between the *debug* mode and the
*release* mode in step 1, while a multi-config generator does so in step 3.

<!-- TODO: Expand on Conan profiles and generators -->

### 1. Preparing build environment with Conan

If you are using a single-config generator, you should pick whether you are
building in the debug mode or the release mode now by running one of the two
commands:

- debug:
  
  ```bash
  conan install . --build=missing -of build -s build_type=Debug
  ```

- release:

  ```bash
  conan install . --build=missing -of build -s build_type=Release
  ```

If you are using a multi-config generator, you should run both commands above.

#### Using modules vs using headers

By default, this library will be built for a consumer that uses C++ modules.
To use headers instead, append `-o use_modules=False` to the above command.
For example, to use headers in a debug build:

```bash
conan install . --build=missing -of build -o use_modules=False -s build_type=Debug
```

### 2. Configuring options with CMake

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

### 3. Compiling code with CMake

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

## Usage

### Basic Example

```cpp
#include "sorted_array_tower/sorted_array_tower.hpp"

int main() {
    sorted_array_tower::SortedArrayTower<int> arr;
    
    arr.insert(5);
    arr.insert(2);
    arr.insert(8);
    
    if (arr.contains(5)) {
        std::cout << "Found 5!" << std::endl;
    }
    
    arr.remove(2);
    
    // Iterate in sorted order
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    
    return 0;
}
```

## Data Structure: SortedArrayTower

The `SortedArrayTower<T>` class maintains elements in sorted order and provides:

- **`insert(value)`** - Insert while maintaining order (O(n) amortized)
- **`remove(value)`** - Remove first occurrence (O(n))
- **`find(value)`** - Binary search for element (O(log n))
- **`contains(value)`** - Check existence (O(log n))
- **`at(index)`** - Access by index (O(1))
- **`min()` / `max()`** - Get extremal values (O(1))
- **Iterator support** - Range-based for loops and STL algorithms

### Template Requirements

`T` must satisfy `std::totally_ordered` (C++20 concept):
- Supports operators: `<`, `>`, `<=`, `>=`, `==`, `!=`

## Testing

Tests are written using the [doctest](https://github.com/doctest/doctest) framework.

```bash
# Run all tests
ctest --test-dir build --output-on-failure

# Run tests with verbose output
./build/test_sorted_array_tower --verbose
```

Test suites cover:
- Basic operations (insertion, construction)
- Search operations (find, contains)
- Deletion operations (remove, clear)
- Min/max operations
- Iteration
- Edge cases and error conditions

## Project Structure

```
.
├── CMakeLists.txt              # Main build configuration
├── conanfile.py                # Conan dependency management
├── README.md                   # This file
├── include/
│   └── sorted_array_tower/
│       └── sorted_array_tower.hpp    # Main header file
├── src/
│   └── sorted_array_tower.cppm       # Implementation (templates + instantiations)
├── tests/
│   └── test_sorted_array_tower.cpp   # Test suite using doctest
└── build/                      # Build directory (created by CMake)
```

## Compilation Modes

### Header-Only (Default)

- Everything is defined in headers
- Flexible, allows specialization per translation unit
- Slightly longer compilation times

To use:
```cpp
#include "sorted_array_tower/sorted_array_tower.hpp"
```

### C++20 Modules

Enable with CMake option:
```bash
cmake --preset conan-default -DSAT_USE_MODULES=ON
```

Modules provide:
- Faster compilation (module interfaces compiled once)
- Better encapsulation
- Cleaner dependency tracking

## License

MIT - See LICENSE file for details

## Development

To extend the library:

1. Add new data structures in `include/sorted_array_tower/`
2. Add implementations in `src/`
3. Add tests in `tests/`
4. Update CMakeLists.txt with new targets
5. Run tests to verify

## Performance Notes

- **Insertion**: $O((\log n)^2)$ time, amortized
- **Lookup**: $O((\log n)^2)$ time, worst case
- **Removal**: $O((\log n)^2)$ time, amortized
- **Memory**: $\approx 2n$
