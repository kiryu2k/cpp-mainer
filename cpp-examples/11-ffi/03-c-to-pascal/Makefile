all: bin/pascal-from-c

bin/pascal-from-c: obj/math.o obj/mystring.o obj/main.o
	$(CC) -o $@ $^

obj/math.o: src/math.pas
	fpc -a -FEobj $^

obj/main.o: src/main.c
	$(CC) -Wall -c -o $@ $^

obj/mystring.o: src/mystring.pas
	fpc -a -FEobj $^

.PHONY: clean
clean:
	rm -f bin/pascal-from-c
	rm -f obj/*.o obj/*.s obj/*.ppu
