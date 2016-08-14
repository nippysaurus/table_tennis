# Table Tennis

Table Tennis is a [Pebble](https://www.pebble.com/) app for keeping track of your table tennis score while you play.

# Dev

## OS X

Install the following brew packages:

- pebble-qemu
- pebble-sdk
- pebble-toolchain

Then simply `make build` to build the package, then `make run_basalt` to run the app in qemu.

There is also a small test suite to check the game logic which can be run with `make test`.
