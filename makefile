all:
	g++ -Isrc/Include -Lsrc/lib -o main src/BaseObject.cpp main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

run:
	./main
