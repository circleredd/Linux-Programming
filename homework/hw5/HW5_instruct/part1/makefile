CC = gcc
CFLAGS = -Wall
TARGET = hostinfo

.PHONY = all clean

all: $(TARGET)

$(TARGET): hostinfo.c
	$(CC) $(CFLAGS) -o $(TARGET) hostinfo.c

# $(TARGET): shell.o parse.o
# 	$(CC) $(CFLAGS) -o $(TARGET) shell.o parse.o

# shell.o:
# 	$(CC) $(CFLAGS) -c shell.c

# parse.o:
# 	$(CC) $(CFLAGS) -c parse.c


clean:
	rm -f $(TARGET)


