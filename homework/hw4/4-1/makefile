CC = gcc
CFLAGS = -Wall
TARGET = userProcess

.PHONY = all clean

all: $(TARGET)

$(TARGET): userProcess.c
	$(CC) $(CFLAGS) -o $(TARGET) userProcess.c

# $(TARGET): shell.o parse.o
# 	$(CC) $(CFLAGS) -o $(TARGET) shell.o parse.o

# shell.o:
# 	$(CC) $(CFLAGS) -c shell.c

# parse.o:
# 	$(CC) $(CFLAGS) -c parse.c


clean:
	rm -f $(TARGET)


