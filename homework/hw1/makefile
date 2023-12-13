CC = gcc
CFLAGS = -Wall
TARGET = demo 

.PHONY: all
.PHONY: clean

all: $(TARGET)

$(TARGET): demo.c
	$(CC) $(CFLAGS) -o $(TARGET) demo.c

clean:
	rm -f $(TARGET)

# main.out: main.c
# 	gcc -o main.out main.c
# clean:
# 	rm -f main.out 
