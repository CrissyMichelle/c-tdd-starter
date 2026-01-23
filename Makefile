CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c11 -g

SRC = src/client.c
UNITY = tests/unity.c

TEST_BINS = test_client_args test_client_connect

.PHONY: test clean

test: $(TEST_BINS)
	./test_client_args
	./test_client_connect

test_client_args:
	$(CC) $(CFLAGS) $(SRC) tests/test_client_args.c $(UNITY) -o test_client_args

test_client_connect:
	$(CC) $(CFLAGS) $(SRC) tests/test_client_connect.c $(UNITY) -o test_client_connect -lpthread

clean:
	rm -f $(TEST_BINS)
