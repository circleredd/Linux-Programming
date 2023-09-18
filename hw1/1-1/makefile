CC = gcc
CFLAGS = -Wall
TARGET = tee 

.PHONY: all
.PHONY: clean

all: $(TARGET)

$(TARGET): tee.c
	$(CC) $(CFLAGS) -o $(TARGET) tee.c

clean:
	rm -f $(TARGET)

# main.out: main.c
# 	gcc -o main.out main.c
# clean:
# 	rm -f main.out 
