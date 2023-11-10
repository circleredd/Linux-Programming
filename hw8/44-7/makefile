TARGET1=nonblocking_server
TARGET2=client
GCC=gcc

ALL: $(TARGET1) $(TARGET2)

$(TARGET1): $(TARGET1).c 
	$(GCC) -o $(TARGET1) $(TARGET1).c 

$(TARGET2): $(TARGET2).c 
	$(GCC) -o $(TARGET2) $(TARGET2).c 

clean:
	rm -f $(TARGET1)
