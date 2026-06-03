# Docker Setup for sorted_array_tower

This directory contains Docker configuration for building, testing, and generating coverage reports for the sorted_array_tower project across different environments.

## Features

- **Multi-stage builds** for efficient testing of all compiler/config combinations
- **CI/CD ready** with results extraction for GitHub Actions integration
- **Coverage reports** in standard HTML format
- **Multiple base images** support (Ubuntu, Debian, Fedora, etc.)

## Quick Start

### Build and test with Ubuntu 24.04 (default)
```bash
docker build -t sorted-tower:ubuntu .
```

### Build and test with different base images
```bash
# Ubuntu 24.04 LTS (recommended)
docker build -t sorted-tower:ubuntu --build-arg BUILD_BASE_IMAGE=ubuntu:24.04 .

# Debian Bookworm
docker build -t sorted-tower:debian --build-arg BUILD_BASE_IMAGE=debian:bookworm .

# Fedora 40 (note: requires Dockerfile.fedora)
# docker build -t sorted-tower:fedora -f Dockerfile.fedora --build-arg BUILD_BASE_IMAGE=fedora:40 .
```

## Using docker-compose

Build and test all distributions at once:
```bash
# Build Ubuntu and Debian (currently working)
docker-compose build ubuntu debian

# Or build a specific service
docker-compose build ubuntu
```

Run and display results:
```bash
docker-compose run ubuntu
docker-compose run debian
```

## What the Docker build does

The main Dockerfile performs the following steps:

1. **Installs dependencies**:
   - Build tools: build-essential, cmake, ninja-build, git
   - Compilers: gcc, g++, clang, clang-tools
   - Coverage tools: gcovr (and llvm-cov for clang)
   - Python and Conan package manager

2. **Builds and tests with all supported configurations**:

   **GCC:**
   - Single-config debug: `gcc-debug`
   - Single-config release: `gcc-release`
   - Multi-config debug/release: `gcc-multi-debug` + `gcc-multi-release`

   **Clang:**
   - Single-config debug: `clang-debug`
   - Single-config release: `clang-release`
   - Multi-config debug/release: `clang-multi-debug` + `clang-multi-release`

3. **Generates coverage report**:
   - Builds with Clang single-config debug (`clang-debug`)
   - Enables coverage instrumentation (`-DSAT_ENABLE_COVERAGE=ON`)
   - Generates HTML coverage report using `gcovr`

## CI/CD Integration

The Docker setup is designed for GitHub Actions integration using a job-based matrix. Each configuration is run in its own workflow job, so build/test pass-fail status is available directly as a GitHub Actions badge.

### GitHub Actions Badges

Use job-level status badges in your `README.md` to show per-config status. Example badge format:

```md
[![GCC Debug](https://github.com/tunococ/sorted_array_tower/actions/workflows/docker-build.yml/badge.svg?query=branch%3Amain+job%3A%22Build+%26+Test+GCC+Debug%22)](https://github.com/tunococ/sorted_array_tower/actions/workflows/docker-build.yml)
[![GCC Release](https://github.com/tunococ/sorted_array_tower/actions/workflows/docker-build.yml/badge.svg?query=branch%3Amain+job%3A%22Build+%26+Test+GCC+Release%22)](https://github.com/tunococ/sorted_array_tower/actions/workflows/docker-build.yml)
[![GCC Multi](https://github.com/tunococ/sorted_array_tower/actions/workflows/docker-build.yml/badge.svg?query=branch%3Amain+job%3A%22Build+%26+Test+GCC+Multi%22)](https://github.com/tunococ/sorted_array_tower/actions/workflows/docker-build.yml)
[![Clang Debug](https://github.com/tunococ/sorted_array_tower/actions/workflows/docker-build.yml/badge.svg?query=branch%3Amain+job%3A%22Build+%26+Test+Clang+Debug%22)](https://github.com/tunococ/sorted_array_tower/actions/workflows/docker-build.yml)
[![Clang Release](https://github.com/tunococ/sorted_array_tower/actions/workflows/docker-build.yml/badge.svg?query=branch%3Amain+job%3A%22Build+%26+Test+Clang+Release%22)](https://github.com/tunococ/sorted_array_tower/actions/workflows/docker-build.yml)
[![Clang Multi](https://github.com/tunococ/sorted_array_tower/actions/workflows/docker-build.yml/badge.svg?query=branch%3Amain+job%3A%22Build+%26+Test+Clang+Multi%22)](https://github.com/tunococ/sorted_array_tower/actions/workflows/docker-build.yml)
```

### Coverage Artifacts

Coverage HTML is uploaded as a GitHub Actions artifact from the `coverage-report` job in `.github/workflows/docker-build.yml`.

After a workflow run, download the artifact from the GitHub Actions UI to view the coverage report locally.

### Notes

- The Dockerfile no longer stores CI results in `/workspace/ci-results/`.
- CI status is now derived from workflow job success/failure.
- Coverage reports are available as artifacts and do not require workspace persistence.

## Artifact Access

Coverage HTML is uploaded as a GitHub Actions artifact from the `coverage-report` job in `.github/workflows/docker-build.yml`.

To access the coverage report after a workflow run, download the `coverage-report` artifact from the GitHub Actions UI.

If you want to run this locally, build and execute the container directly, then inspect the generated report under the local `build/Debug/coverage_report` directory.

## Dockerfile Behavior

This Dockerfile now provides a build-ready environment only. It no longer saves CI status files inside `/workspace`.

Use GitHub Actions job success and failure to surface pass/fail status for each config, and upload coverage HTML separately as an artifact.
## Customization

### Using only specific compilers

If you want to speed up the build, you can modify the Dockerfile to target specific stages:

```bash
# Build only up to the GCC tests
docker build -t sorted-tower:gcc --target test_gcc .

# Build only up to the Clang tests
docker build -t sorted-tower:clang --target test_clang .

# Build only coverage
docker build -t sorted-tower:coverage --target test_coverage .
```

### Adding additional configurations

To add more configurations (e.g., with modules enabled), modify the Dockerfile and add additional Conan install commands with the `-o use_modules=True` option.

### CI-specific builds

For CI/CD environments, you can create minimal builds that only verify one configuration:

```bash
docker build -t sorted-tower:ci-quick --target test_gcc .
```

## Troubleshooting

### Out of memory during build

The comprehensive build tests all compiler/config combinations, which can use significant memory. If you encounter OOM errors:

1. Increase Docker memory limit
2. Use a specific stage instead of the full build: `--target test_gcc` or `--target test_clang`
3. Run only critical tests in CI, local development in Docker

### Slow builds

- First-time builds download and compile dependencies via Conan. Subsequent builds use cached layers.
- To speed up iterative development, consider running tests locally instead of in Docker.
- Use `DOCKER_BUILDKIT=1` for faster builds

## Supported Base Images

The Dockerfile currently works with Debian/Ubuntu-based images:
- `ubuntu:24.04` (recommended)
- `ubuntu:23.10`
- `debian:bookworm`
- `debian:testing`

Other distributions would require separate Dockerfiles:
- **Fedora/RHEL**: Use `dnf` instead of `apt-get`
- **Alpine**: Use `apk` instead of `apt-get` and requires different package names
- **Arch**: Use `pacman` instead of `apt-get`

You can create `Dockerfile.fedora` or `Dockerfile.alpine` as needed.

## Performance Tips

1. **Use BuildKit for faster builds**: `DOCKER_BUILDKIT=1 docker build ...`
2. **Cache Conan dependencies**: The Dockerfile caches layers, so the first build is slower but subsequent builds are faster
3. **Target specific stages**: Use `--target test_gcc` to skip expensive Clang builds during development
4. **Use local development**: For iterative development, consider running tests locally with `justfile` recipes instead of Docker

## Additional Resources

- See [README.md](README.md) for detailed build instructions
- See [justfile](justfile) for local build recipes
- Conan profiles are in `.pr/` directory
- GitHub Actions workflow: `.github/workflows/docker-build.yml`
