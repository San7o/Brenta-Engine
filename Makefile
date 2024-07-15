build/main.out: src/sources/*.cpp
	gcc src/sources/* -o build/main.out -lpthread -I include/ -I src/headers/ -L lib/ -Wall -lglfw -lGL -ldl -lm -lstdc++ -lassimp

run: build/main.out
	LD_LIBRARY_PATH=${PWD}/lib/:${LD_LIBRARY_PATH} ./build/main.out
