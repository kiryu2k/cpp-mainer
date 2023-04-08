all: bin/sum

bin/sum: obj/sum.o
	ld $^ -o $@ 

obj/sum.o: src/sum.s
	as $^ -o $@

.PHONY: clean
clean:
	rm -f bin/sum obj/*.o
