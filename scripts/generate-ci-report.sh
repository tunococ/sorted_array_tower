#!/bin/bash
# Script to generate a summary report of CI test results
# Usage: ./scripts/generate-ci-report.sh [ci-results-dir]

set -e

CI_DIR="${1:-.}/ci-results"

if [ ! -d "$CI_DIR" ]; then
    echo "Error: CI results directory not found: $CI_DIR"
    exit 1
fi

echo "=========================================="
echo "CI Test Results Summary"
echo "=========================================="
echo ""
echo "Report generated: $(date)"
echo ""

# Function to check test status
check_status() {
    local config=$1
    local type=$2
    local status_file="$CI_DIR/$type/${config}.pass"
    
    if [ -f "$status_file" ]; then
        echo "  ✓ $config"
        return 0
    else
        echo "  ✗ $config (FAILED)"
        return 1
    fi
}

# Build Results
echo "Build Results:"
echo "  GCC:"
check_status "gcc-debug" "build" || true
check_status "gcc-release" "build" || true
check_status "gcc-multi" "build" || true
echo "  Clang:"
check_status "clang-debug" "build" || true
check_status "clang-release" "build" || true
check_status "clang-multi" "build" || true

echo ""
echo "Test Results:"
echo "  GCC:"
check_status "gcc-debug" "test" || true
check_status "gcc-release" "test" || true
check_status "gcc-multi" "test" || true
echo "  Clang:"
check_status "clang-debug" "test" || true
check_status "clang-release" "test" || true
check_status "clang-multi" "test" || true

echo ""
echo "Coverage Report:"
if [ -f "$CI_DIR/coverage/generated.pass" ]; then
    echo "  ✓ Generated"
    if [ -f "$CI_DIR/coverage/report/index.html" ]; then
        echo "  Location: $CI_DIR/coverage/report/index.html"
    fi
else
    echo "  ✗ Not generated"
fi

echo ""
echo "=========================================="

# Exit with error if any tests failed
FAILED=0
for f in "$CI_DIR/build"/*.pass "$CI_DIR/test"/*.pass; do
    if [ ! -f "$f" ] 2>/dev/null; then
        FAILED=1
        break
    fi
done

if [ $FAILED -eq 1 ]; then
    echo "Some tests failed!"
    exit 1
else
    echo "All tests passed!"
    exit 0
fi
