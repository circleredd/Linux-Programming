CC = gcc
CFLAGS = -Wall
TARGET1 = shell

.PHONY: all clean

all: $(TARGET1) 


$(TARGET1): shell.c parse.c builtin.c is_background.c pipe_command.c pipe_present.c redirect_in.c redirect_out.c run_command.c
	$(CC) $(CFLAGS) -o $(TARGET1) shell.c parse.c builtin.c is_background.c pipe_command.c pipe_present.c redirect_in.c redirect_out.c run_command.c


# $(TARGET): shell.o parse.o
# 	$(CC) $(CFLAGS) -o $(TARGET) shell.o parse.o

# shell.o:
# 	$(CC) $(CFLAGS) -c shell.c

# parse.o:
# 	$(CC) $(CFLAGS) -c parse.c


clean:
	rm -f $(TARGET1) 


