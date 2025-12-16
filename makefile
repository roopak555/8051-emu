CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2

SRC = $(wildcard src/*.c)
OUT = emulator

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
