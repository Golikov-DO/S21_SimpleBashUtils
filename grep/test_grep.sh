#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# --- файлы ---
cat > test_file1.txt << 'EOF'
Hello world
This is a test file
hello again
GREP is powerful
grep is a line processor
HELLO WORLD
EOF

cat > test_file2.txt << 'EOF'
Another file
with some content
hello from file2
grep example
EOF

cat > patterns.txt << 'EOF'
hello
grep
EOF

# --- тесты ---
tests=(
"hello test_file1.txt"
"-i hello test_file1.txt"
"-v hello test_file1.txt"
"-c hello test_file1.txt"
"-l hello test_file1.txt test_file2.txt"
"-n hello test_file1.txt"
"-h hello test_file1.txt test_file2.txt"
"-o hello test_file1.txt"
"-e hello test_file1.txt"
"-f patterns.txt test_file1.txt"
"-iv hello test_file1.txt"
"-in hello test_file1.txt"
"-ic hello test_file1.txt"
)

# --- запуск ---
for test in "${tests[@]}"; do
    echo "TEST: grep $test"

    args=($test)

    grep "${args[@]}" > sys.txt 2> sys_err.txt
    ./s21_grep "${args[@]}" > my.txt 2> my_err.txt

    if diff -q sys.txt my.txt > /dev/null; then
        echo -e "${GREEN}OUTPUT OK${NC}"
    else
        echo -e "${RED}OUTPUT FAIL${NC}"
        diff -u sys.txt my.txt
    fi

    if diff -q sys_err.txt my_err.txt > /dev/null; then
        echo -e "${GREEN}NO ERROR${NC}"
    else
        echo -e "${RED}ERROR MISMATCH${NC}"
        diff -u sys_err.txt my_err.txt
    fi

    echo "-----------------------------"
done

# --- stdin ---
echo "TEST: stdin"

echo "hello world" | grep hello > sys.txt
echo "hello world" | ./s21_grep hello > my.txt

if diff sys.txt my.txt > /dev/null; then
    echo -e "${GREEN}STDIN OK${NC}"
else
    echo -e "${RED}STDIN FAIL${NC}"
fi

# --- очистка ---
rm -f test_file1.txt test_file2.txt patterns.txt sys.txt my.txt sys_err.txt my_err.txt

echo "DONE"