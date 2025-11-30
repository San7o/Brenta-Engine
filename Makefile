.PHONY: format
format:
	find include/brenta src examples -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -style=file:utils/.clang-format -i
