TESTS = $(shell find tests -name '*.c')
SOURCES = $(shell find src -name '*.c')

test:
	$(TARGET): $(SOURCES)
		gcc $(SOURCES) $(TESTS) -Wall -lm -o test.o

clean:
	if [ -f test.o ]; then \
		rm test.o; \
	fi
