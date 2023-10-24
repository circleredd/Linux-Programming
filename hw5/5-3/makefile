CC = gcc
CFLAGS = -Wall
TARGET1 = convert
TARGET2 = file_lookup

.PHONY: all clean

all: $(TARGET1) $(TARGET2)


$(TARGET1): convert.c
	$(CC) $(CFLAGS) -o $(TARGET1) convert.c

$(TARGET2): main.c lookup1.c
	$(CC) $(CFLAGS) -o $(TARGET2) main.c lookup1.c


# $(TARGET): shell.o parse.o
# 	$(CC) $(CFLAGS) -o $(TARGET) shell.o parse.o

# shell.o:
# 	$(CC) $(CFLAGS) -c shell.c

# parse.o:
# 	$(CC) $(CFLAGS) -c parse.c


clean:
	rm -f $(TARGET1) 


