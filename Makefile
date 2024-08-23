format:
	find  game ecs engine utils examples -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i
