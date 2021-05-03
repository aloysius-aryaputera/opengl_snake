#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
private:
	void _initialize_pixel(int x_size, int y_size);

public:
	std::vector<std::vector<int>> pixel;	

	Board(int x_size, int y_size);
};

Board::Board(int x_size, int y_size) {
	this -> _initialize_pixel(x_size, y_size);
}

void Board::_initialize_pixel(int x_size, int y_size) {
	for (int i = 0; i < x_size; i++) {
		this -> pixel.push_back(std::vector<int>());
		for (int j = 0; j < y_size; j++) {
			this -> pixel[i].push_back(0);
		}
	}
}

#endif
