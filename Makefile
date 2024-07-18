build/render.out: src/sources/*.cpp
	g++ src/sources/* -o build/render.out -lpthread -I include/ -I src/headers/ -L lib/ -Wall -lglfw -lGL -ldl -lm -lstdc++fs -lassimp

build/game.out: game/sources/*.cpp
	g++ game/**/*.cpp engine/**/*.cpp engine/sources/glad.c -o build/game.out -lpthread -I include/ -I game/headers/ -I engine/headers/ -L lib/ -Wall -lglfw -lGL -ldl -lm -lstdc++fs -lassimp

build/unit.out: tests/**/*.cpp
	g++ tests/**/*.cpp tests/*.cpp engine/**/*.cpp engine/sources/glad.c -o build/unit.out -lpthread -I include/ -I tests/headers/ -I engine/headers/ -L lib/ -Wall -lglfw -lGL -ldl -lm -lstdc++fs -lassimp

render: build/render.out
	LD_LIBRARY_PATH=${PWD}/lib/:${LD_LIBRARY_PATH} ./build/main.out

test: build/unit.out
	LD_LIBRARY_PATH=${PWD}/lib/:${LD_LIBRARY_PATH} ./build/unit.out

game: build/game.out
	LD_LIBRARY_PATH=${PWD}/lib/:${LD_LIBRARY_PATH} ./build/game.out
