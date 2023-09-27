#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

function check_result {
  if [ $? -eq 0 ]; then
    echo -e "${GREEN}Test passed${NC}"
  else
    echo -e "${RED}Test failed${NC}"
  fi
}

# Одиночные флаги
echo -e "Test -e flag"
./s21_grep -e "pattern" file1.txt file2.txt > result.txt
grep -e "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -i flag"
./s21_grep -i "Pattern" file1.txt file2.txt > result.txt
grep -i "Pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -v flag"
./s21_grep -v "pattern" file1.txt file2.txt > result.txt
grep -v "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -c flag"
./s21_grep -c "pattern" file1.txt file2.txt > result.txt
grep -c "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt 
check_result

echo -e "Test -l flag"
./s21_grep -l "pattern" file1.txt file2.txt > result.txt
grep -l "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -n flag"
./s21_grep -n "pattern" file1.txt file2.txt > result.txt
grep -n "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -h flag"
./s21_grep -h "pattern" file1.txt file2.txt > result.txt
grep -h "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -s flag"
./s21_grep -s "pattern" NOTEXIST.txt > result.txt
grep -s "pattern" NOTEXIST.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -f flag"
./s21_grep -f pattern.txt file1.txt file2.txt > result.txt
grep -Ef pattern.txt file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -o flag"
./s21_grep -o "pattern" file1.txt file2.txt > result.txt
grep -o "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

# Парные комбинации флагов

echo -e "Test -iv flags"
./s21_grep -iv pattern file1.txt file2.txt > result.txt
grep -iv "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -in flags"
./s21_grep -in pattern file1.txt file2.txt > result.txt
grep -in "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -ls flags"
./s21_grep -ls pattern file1.txt file2.txt NOTEXIST.txt > result.txt
grep -ls "pattern" file1.txt file2.txt NOTEXIST.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -ch flags"
./s21_grep -ch pattern file1.txt file2.txt > result.txt
grep -ch "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -cv flags"
./s21_grep -cv pattern file1.txt file2.txt > result.txt
grep -cv "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -lv flags"
./s21_grep -lv pattern file1.txt file2.txt > result.txt
grep -lv "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -ho flags"
./s21_grep -ho pattern file1.txt file2.txt > result.txt
grep -ho "pattern" file1.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -nf flags"
./s21_grep -nf pattern.txt file2.txt > result.txt
grep -nf pattern.txt file2.txt > expected.txt
diff result.txt expected.txt
check_result

rm result.txt expected.txt
