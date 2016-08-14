CC=gcc
CFLAGS=-I.

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

src/table_tennis.o:
	gcc -c -o src/table_tennis.o src/table_tennis.c

clean:
	rm -f src/*.o
	rm -f tests/*.o
	rm game_state_tests

test: src/table_tennis.o tests/game_state_tests.o
	$(CC) -o game_state_tests tests/game_state_tests.o src/table_tennis.o /usr/local/lib/libcheck.a $(CFLAGS)
	./game_state_tests
