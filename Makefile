CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c11 -g

SRC = src/math_utils.c
TEST_SRC = tests/test_math_utils.c tests/unity.c

TEST_BIN = test_runner

.PHONY: test clean

test:
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) -o $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(TEST_BIN)
