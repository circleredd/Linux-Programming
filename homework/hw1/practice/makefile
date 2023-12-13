CC = gcc
CFLAGS = -Wall
TARGET = main

.PHONY: all
.PHONY: clean

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

clean:
	rm -f $(TARGET)

# main.out: main.c
# 	gcc -o main.out main.c
# clean:
# 	rm -f main.out 
