all: lab5

lab5: lab5.cpp
	g++ -g -msse -Wl,-V lab5.cpp -o lab5

clean:
	rm lab5