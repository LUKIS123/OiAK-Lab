all: simd_test2 lab4

simd: simd_test2.cpp lab4.cpp
	gcc -g -m64 -ggdb simd_test2.cpp -o simd -lstdc++
	gcc -g -m64 -ggdb lab4.cpp -o lab4 -lstdc++

clean:
	rm simd lab4
