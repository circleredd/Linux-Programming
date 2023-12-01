TARGET1=udp_server
TARGET2=udp_client
GCC=gcc

ALL: $(TARGET1) $(TARGET2) 

$(TARGET1): $(TARGET1).c lookup5.c
	$(GCC) -o $(TARGET1) $(TARGET1).c lookup5.c

$(TARGET2): main.c lookup9.c
	$(GCC) -o $(TARGET2) main.c lookup9.c

clean:
	rm -f $(TARGET1) $(TARGET2)
