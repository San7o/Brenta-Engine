build/main.out: src/sources/*.cpp
	gcc src/sources/* -o build/main.out -lpthread -I include/ -I src/headers/ -Wall -lglfw -lGL -ldl -lm -lstdc++
