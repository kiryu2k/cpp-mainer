bin/sum: obj/main.o obj/sum.o
	$(CXX) -o $@ $^

obj/sum.o: src/sum.c
	$(CC) -c -o $@ $^

obj/main.o: src/main.cpp
	$(CXX) -c -o $@ $^

.PHONY: clean
clean:
	rm -f obj/*.o
	rm -f bin/sum