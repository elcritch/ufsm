
CCFLAGS = --std=c99 -Wall -Wshadow -O2
DEBUG_CFLAGS = $(CCFLAGS) -O0 -g
LFLAGS = -lm

.PHONY = all clean

all: lib test test_pr bench example example2 example3

lib: libtinyexpr.a

libtinyexpr.a: tinyexpr.o
	ar rcs $@ $^

test: test.c tinyexpr.c
	$(CC) $(DEBUG_CFLAGS) -o $@ $^ $(LFLAGS)
	./$@

test_pr: test.c tinyexpr.c
	$(CC) $(DEBUG_CFLAGS) -DTE_POW_FROM_RIGHT -DTE_NAT_LOG -o $@ $^ $(LFLAGS)
	./$@

bench: benchmark.c tinyexpr.c
	$(CC) $(DEBUG_CFLAGS) -o $@ $^ $(LFLAGS)

example: example.c tinyexpr.c
	$(CC) $(DEBUG_CFLAGS) -o $@ $^ $(LFLAGS)

example2: example2.c tinyexpr.c
	$(CC) $(DEBUG_CFLAGS) -o $@ $^ $(LFLAGS)


example3: example3.c tinyexpr.c
	$(CC) $(DEBUG_CFLAGS) -DTE_VALUE_TYPE=int -o $@ $^ $(LFLAGS)

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@

clean:
	rm -f *.o *.a *.exe example example2 example3 bench test_pr test
	rm -Rf *.dSYM
