# Produce file delta using rolling hash

You need cmake and ninja or other build system installed.

For "historical reasons", app lives in subdirectory 03.

To build, go to subdirectory 03.

For example, when using ninja, with first build invocation you need to give -GNinja option flag:

- cmake -GNinja -S<path_to_source> -B<path_to_desired_build_dir>

After subsequent build runs, you can leave -GNinja flag out:

- cmake -S<path_to_source> -B<path_to_desired_build_dir>

There will be teo executables:

- eiger03
	- Standalone app

- eiger_test
	- For eunning test cases

Both produce print info for debugging at the moment

## Running eiger03

App needs three parameters:

1. Original file
1. Update file
1. Delta file for output

## Tests

Just run eiger_test

## Note

- I did not use smart pointers for chunk buffer stuff, shared_pointer namely. I was too busy copying char buffers around to remember smartpointers ... 