#!/bin/bash

# --- Configuration ---
EXE="./mygit.exe"
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

function print_header() {
    echo -e "\n${BLUE}========================================${NC}"
    echo -e "${GREEN}>>> $1${NC}"
    echo -e "${BLUE}========================================${NC}"
}

function check_success() {
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Command succeeded.${NC}"
    else
        echo -e "${RED}[FAIL] Command failed!${NC}"
        exit 1
    fi
}

# --- 1. Clean Slate (Manual Cleanup to avoid Makefile errors) ---
print_header "1. Cleaning Environment"
rm -rf .mygit
rm -f *.txt *.o
# We rely on existing mygit.exe, assuming it's built. 
# If not, try to build, but ignore 'clean' errors.
if [ ! -f "$EXE" ]; then
    echo "Building project..."
    make
fi

# --- 2. Init Edge Cases ---
print_header "2. Edge Case: Init Twice"
$EXE init
check_success
echo "Running init again (should handle gracefully)..."
$EXE init
check_success

# --- 3. Add Edge Cases ---
print_header "3. Edge Case: Add Non-Existent File"
$EXE add ghost_file.txt
# We don't exit here because we expect an error message, but the program shouldn't crash.
echo "(Check above: Should say 'File not found')"

# --- 4. Empty File Handling ---
print_header "4. Edge Case: Empty File"
touch empty.txt
$EXE add empty.txt
check_success
$EXE commit "Committing empty file"
check_success

# --- 5. Multiple Files & Complexity ---
print_header "5. Complex Workflow: Multiple Files"
echo "Content A" > file_a.txt
echo "Content B" > file_b.txt
$EXE add file_a.txt
$EXE add file_b.txt
$EXE commit "Added two files"
check_success

# --- 6. Status: Work in Progress ---
print_header "6. Status: Staged vs Modified"
echo "Original" > wip.txt
$EXE add wip.txt
# Now modify it immediately WITHOUT adding again
echo "Modified" > wip.txt
echo "Checking status (Should show wip.txt as MODIFIED)..."
$EXE status

# --- 7. Status: Deletion ---
print_header "7. Status: Deletion Detection"
rm file_a.txt
echo "Deleted file_a.txt manually."
echo "Checking status (Should show file_a.txt as DELETED)..."
$EXE status

# --- 8. Log Verification ---
print_header "8. Log Verification"
$EXE log

# --- 9. Corruption Test ---
print_header "9. Edge Case: Repository Corruption"
echo "Simulating corruption by deleting .mygit/HEAD..."
rm .mygit/HEAD
$EXE log
echo "(Check above: Should say 'Error: .mygit/HEAD not found' or similar, NOT crash)"

echo -e "\n${GREEN}>>> Rigorous Test Suite Completed! <<<${NC}"