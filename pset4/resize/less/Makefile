CC ?= clang
CFLAGS ?= -fsanitize=integer-divide-by-zero -fsanitize=null -fsanitize=undefined -ggdb3 -O0 -std=c11 -Wall -Werror
LDLIBS ?= -lm

resize: resize.c 
	$(CC) $(CFLAGS) -o resize resize.c $(LDLIBS)

.PHONY: clean
clean:
	rm -f resize
