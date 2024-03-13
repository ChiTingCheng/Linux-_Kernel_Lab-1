CC = gcc
CFLAGS = -O1 -g -Wall


OBJS := sort.o

all: sort

sort: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$@.d $<

clean:
	rm -f $(OBJS)
	rm -f *.dSYM
	rm -f sort
	rm -f .*.o.d
