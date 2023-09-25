CC = gcc
CFLAGS = -Wall
TARGET = tail 

.PHONY: all
.PHONY: clean

all: $(TARGET)

$(TARGET): tail.c
	$(CC) $(CFLAGS) -o $(TARGET) tail.c

clean:
	rm -f $(TARGET)

# main.out: main.c
# 	gcc -o main.out main.c
# clean:
# 	rm -f main.out 
