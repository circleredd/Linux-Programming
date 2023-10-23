CC = gcc
CFLAGS = -Wall
TARGET1 = hostinfo
TARGET2 = mydate
TARGET3 = printdir
TARGET4 = mycat
TARGET5 = pipe_ls

.PHONY: all clean

all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5)

$(TARGET1): hostinfo.c
	$(CC) $(CFLAGS) -o $(TARGET1) hostinfo.c

$(TARGET2): mydate.c
	$(CC) $(CFLAGS) -o $(TARGET2) mydate.c

$(TARGET3): printdir.c
	$(CC) $(CFLAGS) -o $(TARGET3) printdir.c

$(TARGET4): mycat.c
	$(CC) $(CFLAGS) -o $(TARGET4) mycat.c

$(TARGET5): pipe_ls.c
	$(CC) $(CFLAGS) -o $(TARGET5) pipe_ls.c



# $(TARGET): shell.o parse.o
# 	$(CC) $(CFLAGS) -o $(TARGET) shell.o parse.o

# shell.o:
# 	$(CC) $(CFLAGS) -c shell.c

# parse.o:
# 	$(CC) $(CFLAGS) -c parse.c


clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) 	


