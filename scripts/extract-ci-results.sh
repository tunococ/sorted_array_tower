#!/bin/bash
# Script to extract CI results from Docker container
# Usage: ./scripts/extract-ci-results.sh <container-id> [output-dir]

set -e

CONTAINER_ID="${1:?Container ID is required}"
OUTPUT_DIR="${2:-.}/ci-results"

echo "Extracting CI results from container: $CONTAINER_ID"
echo "Output directory: $OUTPUT_DIR"

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Extract CI results
docker cp "$CONTAINER_ID:/workspace/ci-results" "$OUTPUT_DIR/tmp" || true

# Reorganize results
if [ -d "$OUTPUT_DIR/tmp/ci-results" ]; then
    mv "$OUTPUT_DIR/tmp/ci-results"/* "$OUTPUT_DIR/" 2>/dev/null || true
    rmdir "$OUTPUT_DIR/tmp/ci-results" 2>/dev/null || true
fi

rmdir "$OUTPUT_DIR/tmp" 2>/dev/null || true

echo ""
echo "=========================================="
echo "CI Results extracted to: $OUTPUT_DIR"
echo "=========================================="
echo ""
echo "Structure:"
tree -L 2 "$OUTPUT_DIR" 2>/dev/null || find "$OUTPUT_DIR" -type f | head -20

echo ""
echo "Build status files:"
ls -1 "$OUTPUT_DIR/build/"*.pass 2>/dev/null || echo "  (none found)"

echo ""
echo "Test status files:"
ls -1 "$OUTPUT_DIR/test/"*.pass 2>/dev/null || echo "  (none found)"

echo ""
echo "Coverage report location:"
if [ -d "$OUTPUT_DIR/coverage/report" ]; then
    echo "  $OUTPUT_DIR/coverage/report/"
    echo "  Main page: $OUTPUT_DIR/coverage/report/index.html"
else
    echo "  (not found)"
fi
