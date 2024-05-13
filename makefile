img: src/main.cc src/round_to_int.cc src/file_parser.cc src/input_handle.cc src/point.cc src/image_handle.cc src/util.cc
	g++ src/main.cc src/round_to_int.cc src/file_parser.cc src/input_handle.cc src/point.cc src/util.cc src/image_handle.cc -g -Ofast -finline-functions -funroll-loops -std=c++17 -pedantic -Wextra -Wall -O2 -lpthread -lSDL2 -lSDL2_image -o img
