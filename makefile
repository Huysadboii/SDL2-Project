all: build run
build:
	g++ -Isrc/Include -Lsrc/lib -o main src/BaseObject.cpp src/GameMap.cpp src/Player.cpp main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

run:
	./main

clean:
	rm main.exe
