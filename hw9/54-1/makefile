TARGET1=reader
TARGET2=writer
TARGET3=debug
GCC=gcc

ALL: $(TARGET1) $(TARGET2) $(TARGET3) 

$(TARGET1): $(TARGET1).c 
	$(GCC) -o $(TARGET1) $(TARGET1).c 

$(TARGET2): $(TARGET2).c 
	$(GCC) -o $(TARGET2) $(TARGET2).c 

$(TARGET3): $(TARGET3).c 
	$(GCC) -o $(TARGET3) $(TARGET3).c 


clean:
	rm -f $(TARGET1) $(TARGET2) 
