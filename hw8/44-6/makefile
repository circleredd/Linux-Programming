TARGET1=fifo_seqnum_server
TARGET2=fifo_seqnum_client
GCC=gcc

ALL: $(TARGET1) $(TARGET2)

$(TARGET1): $(TARGET1).c  get_num.c error_functions.c
	$(GCC) -o $(TARGET1) $(TARGET1).c get_num.c error_functions.c

$(TARGET2): $(TARGET2).c  get_num.c error_functions.c
	$(GCC) -o $(TARGET2) $(TARGET2).c get_num.c error_functions.c

clean:
	rm -f $(TARGET1)
