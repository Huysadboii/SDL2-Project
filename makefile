all: build run
build:
	g++ -Isrc/Include -Lsrc/lib -o main source/AssistDisplay.cpp source/Display.cpp source/Text.cpp source/Explosion.cpp source/Enemy.cpp source/Bullet.cpp source/Object.cpp source/GameMap.cpp source/Player.cpp main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

run:
	./main

clean:
	rm main.exe
