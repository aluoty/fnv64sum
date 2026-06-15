CC = gcc
CFLAGS = -Wall -Wextra -O3
TARGET = fnv64sum

all: $(TARGET)

$(TARGET): src/main.c
	$(CC) $(CFLAGS) src/main.c -o $(TARGET)

clean:
	rm -f $(TARGET)
