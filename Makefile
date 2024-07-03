build/main.out: src/sources/main.cpp src/sources/glad.c src/sources/shader.cpp
	gcc src/sources/main.cpp src/sources/glad.c src/sources/shader.cpp -o build/main.out -lpthread -I include/ -Wall -lglfw -lGL -ldl -lm -lstdc++
