all: simd

simd: simd.cpp
	g++ -g -msse -Wl,-V simd.cpp -o simd

clean:
	rm simd