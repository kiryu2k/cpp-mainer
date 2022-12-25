CFLAGS=-Wall -Wextra

bin/repeat: src/main.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f bin/repeat
