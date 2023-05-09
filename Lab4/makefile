all: simd_test2 lab4

simd: simd_test2.cpp lab4.cpp
	gcc -ggdb -gstabs -Wall -msse -lstdc++ simd_test2.cpp -o simd
	gcc -ggdb -gstabs -Wall -msse -lstdc++ lab4.cpp -o lab4

clean:
	rm simd lab4
