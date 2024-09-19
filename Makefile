format:
	find  game engine utils examples -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i
