TARGET1=quiz5
TARGET2=add2
GCC=gcc

ALL: $(TARGET1) $(TARGET2) 

$(TARGET1): $(TARGET1).c 
	$(GCC) -o $(TARGET1) $(TARGET1).c 

$(TARGET2): $(TARGET2).c
	$(GCC) -o $(TARGET2) $(TARGET2).c


clean:
	rm -f $(TARGET1) $(TARGET2)
