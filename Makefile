build/main.out: src/main.cpp src/glad.c
	gcc src/main.cpp src/glad.c -o build/main.out -lpthread -I include/ -Wall -lglfw -lGL -ldl
