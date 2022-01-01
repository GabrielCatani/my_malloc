CC=
DEBUG=
ifeq ($(shell uname -s), Darwin)
  CC = clang
  DEBUG = -g3 -fsanitize=leak
endif
ifeq ($(shell uname -s), Linux)
  CC = gcc
  DEBUG = -g3 -fsanitize=address
endif

TARGET=my_malloc
INC= -I ./inc/
FLAGS= -Wall -Wextra -Werror
SRC_PATH= ./src/
OBJS_SRC_PATH= ./src_objs/
TEST=./main.c

SRC := $(wildcard $(SRC_PATH)*.c)
SRC_NAMES := $(notdir $(SRC))
SRC_OBJS := $(addprefix $(OBJS_SRC_PATH), $(SRC_NAMES:.c=.o))

all: $(TARGET)

$(TARGET): $(SRC)
	@$(CC) $(FLAGS) $(DEBUG) $(INC) $(SRC) -c -o $@

test: $(SRC_OBJS)
	@$(CC) $(FLAGS) $(DEBUG) $(INC) $(TEST) $(SRC_OBJS) -o $@

$(OBJS_SRC_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJS_SRC_PATH)
	@$(CC) $(FLAGS) $(DEBUG) $(INC) -c $< -o $@

clean:
	@/bin/rm -rf $(OBJS_SRC_PATH)

fclean: clean
	@/bin/rm -rf $(TARGET) test

re: fclean $(TARGET)

.PHONY: all clean fclean re
