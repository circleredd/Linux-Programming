CC = gcc
CFLAGS = -Wall
TARGET = shell

.PHONY = all clean

all: $(TARGET)

$(TARGET): shell.c parse.c builtin.c is_background.c run_command.c
	$(CC) $(CFLAGS) -o $(TARGET) shell.c parse.c builtin.c is_background.c run_command.c

# $(TARGET): shell.o parse.o
# 	$(CC) $(CFLAGS) -o $(TARGET) shell.o parse.o

# shell.o:
# 	$(CC) $(CFLAGS) -c shell.c

# parse.o:
# 	$(CC) $(CFLAGS) -c parse.c


clean:
	rm -f $(TARGET)


