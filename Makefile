build/main.out: src/sources/*.cpp
	gcc src/sources/* -o build/main.out -lpthread -I include/ -I src/headers/ -L lib/ -Wall -lglfw -lGL -ldl -lm -lstdc++ -lassimp

build/engine.out: engine/*.cpp
	gcc engine/* -o build/engine.out -lpthread -I include/ -I engine/headers/ -L lib/ -Wall -lglfw -lGL -ldl -lm -lstdc++ -lassimp

build/game.out: game/*.cpp
	gcc game/* -o build/game.out -lpthread -I include/ -I game/headers/ -L lib/ -Wall -lglfw -lGL -ldl -lm -lstdc++ -lassimp

build/unit.out: tests/*.cpp
	gcc tests/* -o build/unit.out -lpthread -I include/ -I tests/ -L lib/ -Wall -lglfw -lGL -ldl -lm -lstdc++ -lassimp

run: build/main.out
	LD_LIBRARY_PATH=${PWD}/lib/:${LD_LIBRARY_PATH} ./build/main.out

test: build/unit.out
	LD_LIBRARY_PATH=${PWD}/lib/:${LD_LIBRARY_PATH} ./build/unit.out

run-game: build/game.out
	LD_LIBRARY_PATH=${PWD}/lib/:${LD_LIBRARY_PATH} ./build/game.out

run-engine: build/engine.out
	LD_LIBRARY_PATH=${PWD}/lib/:${LD_LIBRARY_PATH} ./build/engine.out
