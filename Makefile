CC=gcc
CFLAGS=-I.

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

src/table_tennis.o:
	gcc -c -o src/c/table_tennis.o src/c/table_tennis.c

%.pdc: %.svg
	python tools/svg2pdc.py $< -o $@

clean:
	rm -f src/*.o
	rm -f tests/*.o
	rm -f game_state_tests
	rm -f resources/images/*.pdc
	pebble clean

build: resources/images/loser_icon.pdc resources/images/winner_icon.pdc
	pebble build

run_basalt:
	pebble install --emulator basalt

run_aplite:
	pebble install --emulator aplite

run_chalk:
	pebble install --emulator chalk

run_diorite:
	pebble install --emulator diorite

test: src/table_tennis.o tests/game_state_tests.o
	$(CC) -o game_state_tests tests/game_state_tests.o src/c/table_tennis.o /usr/local/lib/libcheck.a $(CFLAGS)
	./game_state_tests
