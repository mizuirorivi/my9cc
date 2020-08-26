CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

mizuirocc: $(OBJS)
	$(CC) -o mizuirocc $(OBJS) $(LDFLAGS) 
test: mizuirocc
	./test.sh
clean:
	rm -r mizuirocc *.o *~ tmp*

.PHONY: test clean
