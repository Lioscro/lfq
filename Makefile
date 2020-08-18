.PHONY: build clean cmake test

build:
	mkdir -p build && cd build && cmake .. && make

clean:
	rm -rf build

test:
	mkdir -p build && cd build && cmake .. -DTest=ON && make test && tests/test
