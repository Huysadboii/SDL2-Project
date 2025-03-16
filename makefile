all: build run
build:
	g++ -Isrc/Include -Lsrc/lib -o main source/Enemy.cpp source/Bullet.cpp source/BaseObject.cpp source/GameMap.cpp source/Player.cpp main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

run:
	./main

clean:
	rm main.exe
