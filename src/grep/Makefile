SHELL := /bin/bash
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
FILE = *.txt
REG_FILE = regex.txt

SRC_grep = grep.c
OBJ_grep =  $(SRC_grep:.c=.o)

TARGET_grep = s21_grep

all: s21_grep 

s21_grep: $(OBJ_grep)
	$(CC) $(CFLAGS) $(OBJ_grep) -o $(TARGET_grep)

tests:
	-diff <(./s21_grep He $(FILE)) <(grep He $(FILE))
	-diff <(./s21_grep -v He $(FILE)) <(grep -v He $(FILE))
	-diff <(./s21_grep -i HE $(FILE)) <(grep -i HE $(FILE))
	-diff <(./s21_grep -iv He $(FILE)) <(grep -iv He $(FILE))
	-diff <(./s21_grep -in He $(FILE)) <(grep -in He $(FILE))
	-diff <(./s21_grep -ie "^he" $(FILE)) <(grep -ie "^he" $(FILE))
	-diff <(./s21_grep -ie "he" $(FILE)) <(grep -ie "he" $(FILE))
	-diff <(./s21_grep -ince "he" $(FILE)) <(grep -ince "he" $(FILE))
	-diff <(./s21_grep -lh se $(FILE)) <(grep -lh se $(FILE))
	-diff <(./s21_grep -hc e $(FILE)) <(grep -hc e $(FILE))
	-diff <(./s21_grep -e He -e es $(FILE)) <(grep -e He -e es $(FILE))
	-diff <(./s21_grep -s he not_exist) <(grep -s he not_exist)
	-diff <(./s21_grep -f $(REG_FILE) $(FILE)) <(grep -f $(REG_FILE) $(FILE))
	-diff <(./s21_grep -oi he $(FILE)) <(grep -oi he $(FILE))
	-diff <(./s21_grep -ohf $(REG_FILE) $(FILE)) <(grep -ohf $(REG_FILE) $(FILE))
	-diff <(./s21_grep -oinc he $(FILE)) <(grep -oinc he $(FILE))
	-diff <(./s21_grep -oihlnc -e he -e se -f $(REG_FILE) $(FILE)) <(grep -oihlnc -e he -e se -f $(REG_FILE) $(FILE))
	
cpp:  
		cppcheck --enable=all --suppress=missingIncludeSystem *.c

leaks: 
		valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./s21_grep he test_grep.txt
		valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./s21_grep -e he -e se -f regex.txt test_grep.txt
		valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./s21_grep -ince "he" test_grep.txt
		valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./s21_grep -oinc he test_grep.txt
		valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./s21_grep -lhnc he test_grep.txt

clang: 
		clang-format -i *.c
		clang-format -n *.c

checks: 
		 $(MAKE) cpp clang leaks

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f *.o *.a *.out *.so. ../*/*.o ../*/*.a ../*/*.so

rebuild: clean all 

.PHONY: all clean rebuild s21_grep tests cpp leaks clang checks