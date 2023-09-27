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

echo -e "Test -b flag\n"
./s21_cat -b file1.txt > result.txt
cat -b file1.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -e flag\n"
./s21_cat -e file1.txt > result.txt
cat -e file1.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -n flag\n"
./s21_cat -n file1.txt > result.txt
cat -n file1.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -s flag\n"
./s21_cat -s file1.txt > result.txt
cat -s file1.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -t flag\n"
./s21_cat -t file1.txt > result.txt
cat -t file1.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -E flag\n"
./s21_cat -E file1.txt > result.txt
cat -ev file1.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -T flag\n"
./s21_cat -T file1.txt > result.txt
cat -tv file1.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test --number-nonblank flag\n"
./s21_cat --number-nonblank file1.txt > result.txt
cat -b file1.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test --number flag\n"
./s21_cat --number file1.txt > result.txt
cat -n file1.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test --squeeze-blank flag\n"
./s21_cat --squeeze-blank file1.txt > result.txt
cat -s file1.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -ne flags\n"
./s21_cat -ne file1.txt > result.txt
cat -ne file1.txt > expected.txt
diff result.txt expected.txt
check_result

echo -e "Test -tbs flags\n"
./s21_cat -tbs file1.txt > result.txt
cat -tbs file1.txt > expected.txt
diff result.txt expected.txt
check_result

rm result.txt expected.txt
