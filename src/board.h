#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "snake.h"

class Board {
private:
	//Snake snake;

	void _initialize_pixel();
	void _initialize_snake();
	void _clear_board();
	void _sync_board();

public:
	std::vector<std::vector<int>> pixel;
	int x_size, y_size;	
	Snake snake;

	Board(int x_size_, int y_size_);
	void move_snake();
	void change_snake_direction(int new_direction);
};

Board::Board(int x_size_, int y_size_) {
	this -> x_size = x_size_;
	this -> y_size = y_size_;

	this -> _initialize_pixel();
	this -> _initialize_snake();
}

void Board::change_snake_direction(int new_direction) {
	this -> snake.change_direction(new_direction);
}

void Board::move_snake() {
	this -> snake.move();
}

void Board::_initialize_snake() {
	int x_tail, y_tail, x_head, y_head;
	std::vector<int> x_init, y_init;

	x_tail = int(this -> x_size / 2);
	y_tail = int(this -> y_size / 2);
	
	x_head = x_tail + 1;
	y_head = y_tail;

	x_init.push_back(x_tail);
	x_init.push_back(x_head);
	x_init.push_back(x_head + 1);

	y_init.push_back(y_tail);
	y_init.push_back(y_head);
	y_init.push_back(y_head);

	this -> snake = Snake(x_init, y_init);
	this -> _sync_board();
}

void Board::_initialize_pixel() {
	for (int i = 0; i < this -> x_size; i++) {
		this -> pixel.push_back(std::vector<int>());
		for (int j = 0; j < this -> y_size; j++) {
			this -> pixel[i].push_back(0);
		}
	}
}

void Board::_clear_board() {
	for (int i = 0; i < this -> x_size; i++) {
		for (int j = 0; j < this -> y_size; j++) {
			this -> pixel[i][j] = 0;
		}
	}
}

void Board::_sync_board() {
	this -> _clear_board();

	for (int i = 0; i < this -> snake.x.size(); i++) {
		this -> pixel[this -> snake.x[i]][this -> snake.y[i]] = 1;
	}	
}

#endif
