.PHONY: build build-test check clean compile-release compile-release-windows test test-windows bump-patch bump-minor bump-major push-release

RELEASE_OS ?= local
RELEASE_VERSION ?= local

build:
	mkdir -p build \
	&& cd build \
	&& cmake .. -DCMAKE_BUILD_TYPE=Release \
	&& make

build-test:
	mkdir -p build && \
	cd build && \
	cmake .. -DTest=ON -DCMAKE_BUILD_TYPE=Debug && \
	make

compile-release:
	mkdir -p release/lfq
	cp -rf build/src/lfq release/lfq/
	cp -rf tests/fixtures/fastq/example.fastq release/lfq/
	cp -rf LICENSE release/lfq/
	cp -rf README.md release/lfq/
	cd release \
	&& tar -czvf lfq_${RELEASE_OS}-${RELEASE_VERSION}.tar.gz lfq

compile-release-windows:
	mkdir -p release/lfq
	cp -rf build/src/lfq.exe release/lfq/
		cp -rf tests/fixtures/fastq/example.fastq release/lfq/
	cp -rf LICENSE release/lfq/
	cp -rf README.md release/lfq/
	cd release \
	&& zip -r -X lfq_${RELEASE_OS}-${RELEASE_VERSION}.zip lfq

check:
	pre-commit run --all-files --verbose

clean:
	rm -rf build
	rm -rf release
	rm -rf ext

test: build-test
	cd build \
	&& tests/test
	nosetests --verbose tests.test_lfq

test-windows:
	cd build \
	&& tests/test.exe
	nosetests --verbose tests.test_lfq

bump-patch:
	bumpversion patch

bump-minor:
	bumpversion minor

bump-major:
	bumpversion major

push-release:
	git push && git push --tags
