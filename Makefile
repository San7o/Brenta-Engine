format:
	find  game ecs engine utils -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i
