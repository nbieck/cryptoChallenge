CC = clang-3.8
FLAGS = -std=c++11
SOURCE = encoding.cpp

all:
	$(CC) $(SOURCE) $(FLAGS)
