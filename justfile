# Remove build artifacts
clean:
    rm -rf build

# Detect Conan profile
detect:
    conan profile detect --force

# Initialize single-config build
init-single compiler='gcc' build_type='debug' coverage=( if build_type == 'debug' { 'cov' } else { '' } ):
    conan install . --build=missing -pr .pr/{{ compiler }}-{{ build_type }} \
    && cmake --preset conan-{{ build_type }} \
    -DSAT_ENABLE_COVERAGE={{ if coverage =~ 'cov' { 'ON' } else { 'OFF' } }}

# Initialize multi-config build
init-multi compiler='gcc' coverage='':
    conan install . --build=missing -pr .pr/{{ compiler }}-multi-debug \
    && conan install . --build=missing -pr .pr/{{ compiler }}-multi-release \
    && cmake --preset conan-default \
    -DSAT_ENABLE_COVERAGE={{ if coverage =~ 'cov' { 'ON' } else { 'OFF' } }}

alias init := init-single

# Build code
build build_type='debug':
    cmake --build --preset conan-{{ build_type }}

# Run tests
test build_type='debug':
    ctest --preset conan-{{ build_type }}

# Run tests and generate a coverage report
build-cov build_type='debug':
    cmake --build --preset conan-{{ build_type }} --target coverage

# Show the coverage report in a web browser
show-cov build_type='debug':
    python3 -m http.server --directory build/{{ if build_type == 'release' { 'Release' } else { 'Debug' } }}/coverage_report 8080
