#!/bin/bash

set -euo pipefail

# Define the directory with test files
TEST_DIR="$PWD/src/test/c/accept"
SCRIPT_PATH="script/ubuntu"

# Build the project
cd "$SCRIPT_PATH"
./build.sh > /dev/null 2>&1  # Suppress all logs during the build process

# Iterate over all files in the test directory
for test_file in "$TEST_DIR"/*; do
    if [[ -f "$test_file" ]]; then
        echo "=================================================="
        ./start.sh "$test_file" "$@" | grep -v "DEBUG"| sed '/^$/d'   # Filter out debug logs
    fi
done

