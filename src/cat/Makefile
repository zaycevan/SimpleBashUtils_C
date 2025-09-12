SHELL := /bin/bash
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
FILE = test.txt

SRC_cat = cat.c
OBJ_cat =  $(SRC_cat:.c=.o)
#OBJ_cat = $(patsubst %.c, %.o, $(notdir $(SRC)))

TARGET_cat = s21_cat

all: s21_cat 

s21_cat: $(OBJ_cat)
	$(CC) $(CFLAGS) $(OBJ_cat) -o $(TARGET_cat)

tests:
	-diff <(./s21_cat $(FILE)) <(cat $(FILE))
	-diff <(./s21_cat -b $(FILE)) <(cat -b $(FILE))
	-diff <(./s21_cat --number-nonblank $(FILE)) <(cat -b $(FILE))
	-diff <(./s21_cat -e $(FILE)) <(cat -e $(FILE))
	-diff <(./s21_cat -E $(FILE)) <(cat -E $(FILE))
	-diff <(./s21_cat -n $(FILE)) <(cat -n $(FILE))
	-diff <(./s21_cat --number $(FILE)) <(cat -n $(FILE))
	-diff <(./s21_cat -s $(FILE)) <(cat -s $(FILE))
	-diff <(./s21_cat --squeeze-blank $(FILE)) <(cat -s $(FILE))
	-diff <(./s21_cat -t $(FILE)) <(cat -t $(FILE))
	-diff <(./s21_cat -T $(FILE)) <(cat -T $(FILE))
	-diff <(./s21_cat -etsbn $(FILE)) <(cat -etsbn $(FILE))
	
cpp:  
		cppcheck --enable=all --suppress=missingIncludeSystem *.c

leaks: 
		valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./s21_cat -etsbn test.txt
		valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./s21_cat test.txt

clang: 
		clang-format -i *.c
		clang-format -n *.c

checks: 
		 $(MAKE) cpp clang  leaks

#$(TARGET): $(OBJ) 
#	$(CC) $(CFLAGS) $(OBJ) -o $(TARDIR)$(TARGET) 

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

#$(OBJ): $(SRC)
#	$(CC) $(CFLAGS) -c $(SRC)


clean:
	rm -f *.o *.a *.out *.so. ../*/*.o ../*/*.a ../*/*.so

rebuild: clean all 

.PHONY: all clean rebuild s21_cat tests cpp leaks clang checks