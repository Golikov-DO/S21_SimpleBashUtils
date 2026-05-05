#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# --- подготовка файлов ---
echo -e "Hello\nWorld\n\nTest\n" > test1.txt
echo -e "Line1\n\n\nLine2\n" > test2.txt
echo -n "No newline at end" > test3.txt
touch empty.txt

# --- тесты ---
tests=(
"test1.txt"
"-n test1.txt"
"-b test1.txt"
"-s test2.txt"
"-E test1.txt"
"-T test1.txt"
"-v test1.txt"
"-e test1.txt"
"-t test1.txt"
"-ns test1.txt"
"-bE test1.txt"
"-b test1.txt test2.txt"
"-n test1.txt test2.txt"
)

# --- запуск ---
for test in "${tests[@]}"; do
    echo "TEST: cat $test"

    args=($test)

    cat "${args[@]}" > sys.txt 2> sys_err.txt
    ./s21_cat "${args[@]}" > my.txt 2> my_err.txt

    if diff -q sys.txt my.txt > /dev/null; then
        echo -e "${GREEN}OUTPUT OK${NC}"
    else
        echo -e "${RED}OUTPUT FAIL${NC}"
        diff -u sys.txt my.txt
    fi

    if diff -q sys_err.txt my_err.txt > /dev/null; then
        echo -e "${GREEN}NO ERROR${NC}"
    else
        echo -e "${RED}ERROR OUTPUT MISMATCH${NC}"
        diff -u sys_err.txt my_err.txt
    fi

    echo "-----------------------------"
done

# --- stdin ---
echo "TEST: stdin"

echo "hello stdin" | cat > sys.txt
echo "hello stdin" | ./s21_cat > my.txt

if diff sys.txt my.txt > /dev/null; then
    echo -e "${GREEN}STDIN OK${NC}"
else
    echo -e "${RED}STDIN FAIL${NC}"
fi

# --- очистка ---
rm -f test1.txt test2.txt test3.txt empty.txt sys.txt my.txt sys_err.txt my_err.txt

echo "DONE"