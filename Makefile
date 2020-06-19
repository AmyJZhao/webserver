# to run server: ./build/bin/server sample_config

gcc: # use for local development
	@echo "Installing GCC 8.4.0 locally with apt-get..."
	sudo apt-get install gcc-8 g++-8 -y

install: 
	@echo "Using cmake to generate Makefile for the webserver..."
	cd build && cmake -DCMAKE_BUILD_TYPE=Coverage \
		-DCMAKE_C_COMPILER=/usr/bin/gcc-8 -DCMAKE_CXX_COMPILER=/usr/bin/g++-8 ..

build: install
	@cd build && make

test: build
	@cd build && make test

coverage: build
	@cd build && make coverage

all: coverage