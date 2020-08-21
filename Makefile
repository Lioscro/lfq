.PHONY: build check clean test

build:
	mkdir -p build && cd build && cmake .. && make

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
