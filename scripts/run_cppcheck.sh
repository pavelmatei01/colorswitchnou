#!/bin/bash
set -e  # Exit on error

FILES=(main.cpp elemente.cpp joc.cpp nivel.cpp obiectjoc.cpp player.cpp)
OUTPUT_FILE="cppcheck_report.txt"

# Run cppcheck with suppressions, only fail on [error]
cppcheck \
    --std=c++17 \
    --enable=all \
    --inconclusive \
    --suppress=normalCheckLevelMaxBranches \
    --error-exitcode=0 \
    --template="{file}:{line}:{column}: {severity}: {message} [{id}]" \
    --define=GITHUB_ACTIONS=1 \
    --define=SFML_STATIC=1 \
    "${FILES[@]}" 2>&1 | tee "$OUTPUT_FILE"

# Check for errors only
if grep -q "\[error\]" "$OUTPUT_FILE"; then
    echo "::error::Cppcheck errors found! See $OUTPUT_FILE"
    exit 1
else
    echo "::notice::Cppcheck passed (warnings/styles ignored)."
    # Optional: Upload report as artifact
    # echo "::upload-artifact::Cppcheck Report::$OUTPUT_FILE"
fi