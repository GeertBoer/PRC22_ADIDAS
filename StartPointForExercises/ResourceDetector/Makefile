UNITY_FOLDER=../Unity
TEST_INC_DIRS=$(INC_DIRS) -I$(UNITY_FOLDER)

TEST_FILES=$(UNITY_FOLDER)/unity.c \
           list_test.c \
           list.c

HEADER_FILES=*.h

TEST = list_test

CC=gcc

SYMBOLS=-Wall -Werror -pedantic -O0 -ggdb -std=c99
TEST_SYMBOLS=$(SYMBOLS) -DTEST

.PHONY: clean test

all: $(TEST)

$(TEST): Makefile $(TEST_FILES)  $(HEADER_FILES)
	$(CC) $(TEST_INC_DIRS) $(TEST_SYMBOLS) $(TEST_FILES) -o $(TEST)

clean:
	rm -f list $(TEST)

test: $(TEST)
	@valgrind ./$(TEST)
