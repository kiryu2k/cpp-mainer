bin/fill: obj/main.o obj/rust/debug/libfill.a
	gcc -o $@ $^

obj/rust/debug/libfill.a: rust/src/lib.rs
	cargo build --manifest-path rust/Cargo.toml --target-dir obj/rust

obj/main.o: src/main.c
	$(CC) -Wall -Wextra -c -o $@ $^

.PHONY: clean
clean:
	rm -f bin/fill
	rm -rf obj/rust
	rm -f obj/*.o