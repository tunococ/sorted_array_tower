#!/bin/bash
# Script to serve coverage report locally
# Usage: ./scripts/serve-coverage.sh [ci-results-dir] [port]

set -e

CI_DIR="${1:-.}/ci-results"
PORT="${2:-8080}"

if [ ! -d "$CI_DIR/coverage/report" ]; then
    echo "Error: Coverage report not found at $CI_DIR/coverage/report"
    exit 1
fi

echo "Starting HTTP server for coverage report..."
echo "URL: http://localhost:$PORT"
echo ""
echo "Coverage report location: $CI_DIR/coverage/report"
echo "Press Ctrl+C to stop"
echo ""

cd "$CI_DIR/coverage/report"
python3 -m http.server "$PORT"
