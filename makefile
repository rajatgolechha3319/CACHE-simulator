all: build

build: cache_simulate.cpp
		g++ cache_simulate.cpp -o cache_simulate

clean:
		rm -rf cache_simulate