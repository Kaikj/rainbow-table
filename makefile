CC=g++
SHA=sha1/sha1.o -Isha1

all: generate b f

generate: Generate.cpp
	$(CC) Generate.cpp $(SHA) -o Generate

b: B.cpp
	$(CC) B.cpp $(SHA) -o B

f: F.cpp
	$(CC) F.cpp $(SHA) -o F

.PHONY: clean

clean:
	rm -f Generate B F