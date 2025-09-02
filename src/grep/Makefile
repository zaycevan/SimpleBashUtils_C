CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

DIR_cat = ./cat/
DIR_grep = ./grep/

SRC_cat = $(DIR_cat)*.c
SRC_grep = $(DIR_grep)*.c

OBJ_cat =  $(SRC_cat:.c=.o)
OBJ_grep = $(SRC_grep:.c=.o)
#OBJ_cat = $(patsubst %.c, %.o, $(notdir $(SRC)))
#OBJ_grep = $(patsubst %.c, %.o, $(notdir $(SRC)))

TARGET_cat = s21_cat
TARGET_grep = s21_grep

all: s21_cat s21_grep

s21_cat: $(OBJ_cat)
	$(CC) $(CFLAGS) $(OBJ_cat) -o $(DIR_cat)$(TARGET_cat)
s21_grep: $(OBJ_grep)
	$(CC) $(CFLAGS) $(OBJ_grep) -o $(DIR_grep)$(TARGET_grep)


#$(TARGET): $(OBJ) 
#	$(CC) $(CFLAGS) $(OBJ) -o $(TARDIR)$(TARGET) 

$(DIR_cat)%.o: $(DIR_cat)%.c 
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_grep)%.o: $(DIR_grep)%.c 
	$(CC) $(CFLAGS) -c $< -o $@

#$(OBJ): $(SRC)
#	$(CC) $(CFLAGS) -c $(SRC)


clean:
	rm -f *.o *.a *.out  *.so. ../*/*.o ../*/*.a ../*/*.so

rebuild: clean all 
