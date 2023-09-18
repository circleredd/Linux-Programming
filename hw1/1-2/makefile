CC = gcc
CFLAGS = -Wall
TARGET = cp 

.PHONY: all
.PHONY: clean

all: $(TARGET)

$(TARGET): cp.c
	$(CC) $(CFLAGS) -o $(TARGET) cp.c

clean:
	rm -f $(TARGET)

# main.out: main.c
# 	gcc -o main.out main.c
# clean:
# 	rm -f main.out 
