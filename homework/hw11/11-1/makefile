TARGET1=usock_server
TARGET2=usock_client
GCC=gcc

ALL: $(TARGET1) $(TARGET2) 

$(TARGET1): $(TARGET1).c lookup5.c
	$(GCC) -o $(TARGET1) $(TARGET1).c lookup5.c

$(TARGET2): main.c lookup7.c
	$(GCC) -o $(TARGET2) main.c lookup7.c

clean:
	rm -f $(TARGET1) $(TARGET2)
