
CCFLAGS = -ansi -Wall -Wshadow -O2 -g 
LFLAGS = -lm

.PHONY = all clean

all: lib test test_pr bench example example2 example3

lib: libtinyexpr.a

libtinyexpr.a: tinyexpr.o
	ar rcs $@ $^

test: test.c tinyexpr.c
	$(CC) $(CCFLAGS) -o $@ $^ $(LFLAGS)
	./$@

test_pr: test.c tinyexpr.c
	$(CC) $(CCFLAGS) -DTE_POW_FROM_RIGHT -DTE_NAT_LOG -o $@ $^ $(LFLAGS)
	./$@

bench: benchmark.o tinyexpr.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LFLAGS)

example: example.o tinyexpr.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LFLAGS)

example2: example2.o tinyexpr.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LFLAGS)

example3: example3.o tinyexpr.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LFLAGS)

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@

clean:
	rm -f *.o *.a *.exe example example2 example3 bench test_pr test
