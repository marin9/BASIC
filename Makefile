CC=gcc
CFLAGS=-Wall -Wextra
SRC=basic.c
TARGET=basic
TEST=test/test7.bas


all: $(TARGET)

run: $(TARGET)
	@./$(TARGET) $(TEST)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm $(TARGET)

