.PHONY: build build-windows check clean test test-windows

build:
	mkdir -p build && \
	cd build && \
	cmake .. -DCMAKE_BUILD_TYPE=RELEASE && \
	make

build-windows:
	mkdir build && \
	cd build && \
	cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=RELEASE && \
	make

check:
	pre-commit run --all-files --verbose

clean:
	rm -rf build

test:
	mkdir -p build && \
	cd build && \
	cmake .. -DTest=ON -DCMAKE_BUILD_TYPE=DEBUG && \
	make && \
	tests/test && \
	../tests/test_encode_decode.py src/lfq ../tests/fixtures/fastq/test.fastq

test-windows:
	mkdir build && \
	cd build && \
	cmake .. -G "MinGW Makefiles" -DTest=ON -DCMAKE_BUILD_TYPE=DEBUG && \
	make && \
	tests/test.exe && \
	python ../tests/test_encode_decode.py src/lfq ../tests/fixtures/fastq/test.fastq
