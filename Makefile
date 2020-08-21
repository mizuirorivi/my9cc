CFLAGS=-std=c11 -g -static

mizuirocc: mizuirocc.c

test: mizuirocc
	./test.sh
clean:
	rm -r mizuirocc *.o *~ tmp*

.PHONY: test clean
