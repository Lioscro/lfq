.PHONY: build build-windows check clean test test-windows

build:
	mkdir -p build && \
	cd build && \
	cmake .. && \
	make

build-windows:
	mkdir build && \
	cd build && \
	cmake .. -G "MinGW Makefiles" && \
	make

check:
	pre-commit run --all-files --verbose

clean:
	rm -rf build

test:
	mkdir -p build && \
	cd build && \
	cmake .. -DTest=ON && \
	make && \
	tests/test && \
	../tests/test_encode_decode.py src/lfq ../tests/fixtures/fastq/test.fastq

test-windows:
	mkdir build && \
	cd build && \
	cmake .. -G "MinGW Makefiles" -DTest=ON && \
	make && \
	tests/test.exe && \
	python ../tests/test_encode_decode.py src/lfq ../tests/fixtures/fastq/test.fastq
