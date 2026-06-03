# Dockerfile for building and testing sorted_array_tower
# Supports different base images via BUILD_BASE_IMAGE arg
# Designed for CI/CD integration with GitHub Actions
# Usage:
#   docker build -t sorted-tower:ubuntu --build-arg BUILD_BASE_IMAGE=ubuntu:24.04 .
#   docker build -t sorted-tower:debian --build-arg BUILD_BASE_IMAGE=debian:bookworm .
#   docker build -t sorted-tower:fedora --build-arg BUILD_BASE_IMAGE=fedora:40 .

ARG BUILD_BASE_IMAGE=ubuntu:24.04
FROM ${BUILD_BASE_IMAGE} as builder

# Install dependencies based on base image
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    cmake \
    ninja-build \
    python3 \
    python3-pip \
    clang \
    clang-tools \
    gcc \
    g++ \
    gcovr \
    && rm -rf /var/lib/apt/lists/*

# Install Conan via pip
RUN pip3 install conan --break-system-packages

# Create work directory
WORKDIR /workspace

# Copy source code
COPY . .

# Detect Conan profile
RUN conan profile detect --force

CMD ["bash"]
