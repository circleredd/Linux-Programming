TARGET1=sig_speed_sigsuspend
TARGET2=sig_speed_sigwaitinfo
GCC=gcc

ALL: $(TARGET1) $(TARGET2)

$(TARGET1): $(TARGET1).c  error_functions.c get_num.c
	$(GCC) -o $(TARGET1) $(TARGET1).c error_functions.c get_num.c

$(TARGET2): $(TARGET2).c  error_functions.c get_num.c
	$(GCC) -o $(TARGET2) $(TARGET2).c error_functions.c get_num.c

clean:
	rm -f $(TARGET1) $(TARGET2)
